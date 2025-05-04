#include "csapp.h"

#define PORT "8080"
#define SIZE 20

int check_bound(int r, int c){
    return (r >= SIZE || c >= SIZE || r < 0 || c < 0);
}

int check_winner(int** board, int r, int c){
    // check column
    int counter = 0, i = 0;
    
    

    // check row
    // check main diagonali
    // check diagonali 
}

int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];
    
    listenfd = Open_listenfd(PORT);
    printf("Server listening on port %s...\n", PORT);

    int board[SIZE][SIZE] = {0};
    
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        
        Getnameinfo((SA *)&clientaddr, clientlen,
                    client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to client (%s:%s)\n", client_hostname, client_port);
        
        char buffer[MAXLINE];
        sprintf(buffer, "Welcome to XO Game Server!\n");
        Rio_writen(connfd, buffer, strlen(buffer));
        
        Close(connfd);
    }
    
    return 0;
}