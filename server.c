#include "server.h"

int main(int argc, char **argv) {
    int listenfd, connfd[2];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];
    
    listenfd = Open_listenfd(PORT);
    printf("Server listening on port %s...\n", PORT);
    
    while (1) {
        // Ehnii hun
        clientlen = sizeof(struct sockaddr_storage);
        connfd[0] = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        
        Getnameinfo((SA *)&clientaddr, clientlen,
                    client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Player 1 connected (%s:%s)\n", client_hostname, client_port);
        
        char buffer[MAXLINE];
        sprintf(buffer, "Welcome to XO Game Server! You are Player 1 (X). Waiting for Player 2...\n");
        Rio_writen(connfd[0], buffer, strlen(buffer));
        
        // 2dah hun
        clientlen = sizeof(struct sockaddr_storage);
        connfd[1] = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        
        Getnameinfo((SA *)&clientaddr, clientlen,
                    client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Player 2 connected (%s:%s)\n", client_hostname, client_port);
        
        sprintf(buffer, "Welcome to XO Game Server! You are Player 2 (O). Game is starting...\n");
        Rio_writen(connfd[1], buffer, strlen(buffer));
        
        sprintf(buffer, "Player 2 has connected. Game is starting...\n");
        Rio_writen(connfd[0], buffer, strlen(buffer));
        
        handle_game(connfd[0], connfd[1]);

        printf("Game ends!\n");

        Close(connfd[0]);
        Close(connfd[1]);
    }
    
    return 0;
}