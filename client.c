#include "csapp.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "8080"
#define BOARD_SIZE 20

int main(int argc, char **argv) {
    int clientfd;
    char *host, *port;
    char buf[MAXLINE];
    rio_t rio;

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
    Rio_readinitb(&rio, clientfd);
    printf("Connected to server at %s:%s\n", host, port);

    // Read welcome message from server
    Rio_readlineb(&rio, buf, MAXLINE);
    printf("Server says: %s", buf);

    // In a real game, you would:
    // 1. Receive game state (board, player symbol)
    // 2. Update display and get user input
    // 3. Send moves to server
    // 4. Receive updates and display them

    // Simple test to show connection works
    printf("Press Enter to exit...\n");
    if (fgets(buf, MAXLINE, stdin) == NULL) {
        printf("Error reading input or EOF reached\n");
    }
    
    Close(clientfd);
    return 0;
}