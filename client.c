#include "csapp.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "8080"
#define BOARD_SIZE 20

// Function to read and display messages from the server
void process_server_messages(int clientfd) {
    rio_t rio;
    char buf[MAXLINE];
    int game_over = 0;
    
    Rio_readinitb(&rio, clientfd);
    
    while (!game_over) {
        // Read a line from the server
        ssize_t n = Rio_readlineb(&rio, buf, MAXLINE);
        if (n <= 0) {
            printf("Server disconnected\n");
            break;
        }
        
        // Print the message to the console
        printf("%s", buf);
        
        // Check if it's the player's turn to make a move
        if (strstr(buf, "Your turn") != NULL) {
            char input[MAXLINE];
            printf("Enter your move (row,col): ");
            if (fgets(input, MAXLINE, stdin) == NULL) {
                break;
            }
            
            // Send the move to the server
            Rio_writen(clientfd, input, strlen(input));
        }
        
        // Check if the game has ended
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
    
    // Check command line args
    if (argc != 3) {
        host = SERVER_IP;  // default: localhost
        port = SERVER_PORT;  // default: 8080
    } else {
        host = argv[1];
        port = argv[2];
    }
    
    // Connect to the server
    clientfd = Open_clientfd(host, port);
    printf("Connected to server at %s:%s\n", host, port);
    
    // Process server messages until game ends
    process_server_messages(clientfd);
    
    // Close the connection
    Close(clientfd);
    return 0;
}