#include "csapp.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "8080"
#define BOARD_SIZE 20

void process_server_messages(int clientfd) {
    rio_t rio;
    char buf[MAXLINE];
    int game_over = 0;
    
    Rio_readinitb(&rio, clientfd);
    
    while (!game_over) {
        ssize_t n = Rio_readlineb(&rio, buf, MAXLINE);
        if (n <= 0) {
            printf("Server disconnected\n");
            break;
        }
        
        printf("%s", buf);
        
        if (strstr(buf, "Your turn") != NULL) {
            char input[MAXLINE];
            printf("Enter your move (row,col): ");
            fflush(stdout);
        
            if (fgets(input, MAXLINE, stdin) == NULL) {
                break;
            }
            
            size_t len = strlen(input);
            if (len > 0 && input[len-1] == '\n') {
                input[len-1] = '\0';
            }
            
            // printf("Sending move: %s\n", input);
            
            strcat(input, "\n");

            Rio_writen(clientfd, input, strlen(input));
        }
        
        if (strstr(buf, "wins") != NULL || strstr(buf, "draw") != NULL) {
            printf("Game over! Press Enter to exit...\n");
            fgets(buf, MAXLINE, stdin);
            game_over = 1;
        }
    }
}

int main(int argc, char **argv) {
    int clientfd;
    char *host, *port;
    
    if (argc != 3) {
        host = SERVER_IP;
        port = SERVER_PORT;
    } else {
        host = argv[1];
        port = argv[2];
    }
    
    clientfd = Open_clientfd(host, port);
    printf("Connected to server at %s:%s\n", host, port);
    
    process_server_messages(clientfd);
    
    Close(clientfd);
    return 0;
}