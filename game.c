#include "csapp.h"
#include "game.h"

int check_winner(int board[SIZE][SIZE], int row, int col) {
    int player = board[row][col];
    if (player == 0) return 0;
    
    // Baganiin daguu
    int count = 1;
    for (int i = col - 1; i >= 0 && board[row][i] == player && count < 5; i--) count++;
    for (int i = col + 1; i < SIZE && board[row][i] == player && count < 5; i++) count++;
    if (count >= 5) return player;
    
    // Moriin daguu
    count = 1;
    for (int i = row - 1; i >= 0 && board[i][col] == player && count < 5; i--) count++;
    for (int i = row + 1; i < SIZE && board[i][col] == player && count < 5; i++) count++;
    if (count >= 5) return player;
    
    // gol diagonali
    count = 1;
    for (int i = 1; row - i >= 0 && col - i >= 0 && board[row-i][col-i] == player && count < 5; i++) count++;
    for (int i = 1; row + i < SIZE && col + i < SIZE && board[row+i][col+i] == player && count < 5; i++) count++;
    if (count >= 5) return player;
    
    // esreg diagonali
    count = 1;
    for (int i = 1; row - i >= 0 && col + i < SIZE && board[row-i][col+i] == player && count < 5; i++) count++;
    for (int i = 1; row + i < SIZE && col - i >= 0 && board[row+i][col-i] == player && count < 5; i++) count++;
    if (count >= 5) return player;
    
    return 0;
}



void handle_game(int player1_fd, int player2_fd) {
    int board[SIZE][SIZE] = {0};
    int current_player = 1;
    int game_over = 0;
    int row, col;
    char buffer[MAXLINE];
    rio_t rio;
    
    while (!game_over) {
        printf("current player: %d\n", current_player);
        int active_fd = (current_player == 1) ? player1_fd : player2_fd;
        int waiting_fd = (current_player == 1) ? player2_fd : player1_fd;
        
        send_board(player1_fd, board);
        send_board(player2_fd, board);
        
        sprintf(buffer, "TURN:%d, active fd: %d\n", current_player, active_fd);
        Rio_writen(player1_fd, buffer, strlen(buffer));
        Rio_writen(player2_fd, buffer, strlen(buffer));
        
        sprintf(buffer, "Your turn (Player %d).\n", current_player);
        Rio_writen(active_fd, buffer, strlen(buffer));
        
        sprintf(buffer, "Huleej baina Player %d iig\n", current_player);
        Rio_writen(waiting_fd, buffer, strlen(buffer));
        
        Rio_readinitb(&rio, active_fd);
        if (Rio_readlineb(&rio, buffer, MAXLINE) <= 0) {
            printf("Player %d disconnected\n", current_player);
            break;
        }
        
        printf("player %d: %s", current_player, buffer);
        
        
        if (sscanf(buffer, "%d,%d", &row, &col) != 2) {
            sprintf(buffer, "Buruu ugugdul. Use format: row,col\n");
            Rio_writen(active_fd, buffer, strlen(buffer));
            continue;
        }
        
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != 0) {
            sprintf(buffer, "Bolomjgui uildel. Dahin oroldoj uzeerei\n");
            Rio_writen(active_fd, buffer, strlen(buffer));
            continue;
        }
        
        board[row][col] = current_player;
        
        if (check_winner(board, row, col) == current_player) {
            sprintf(buffer, "Player %d wins!\n", current_player);
            Rio_writen(player1_fd, buffer, strlen(buffer));
            Rio_writen(player2_fd, buffer, strlen(buffer));
            game_over = 1;
            continue;
        }
    
        int board_full = 1;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board_full = 0;
                    break;
                }
            }
            if (!board_full) break;
        }
        
        if (board_full) {
            sprintf(buffer, "Game ended in a draw!\n");
            Rio_writen(player1_fd, buffer, strlen(buffer));
            Rio_writen(player2_fd, buffer, strlen(buffer));
            game_over = 1;
            continue;
        }
        
        current_player = (current_player == 1) ? 2 : 1;
    }
}


void send_board(int fd, int board[SIZE][SIZE]) {
    char buffer[MAXLINE];
    char line[SIZE*4 + 2];
    
    sprintf(buffer, "\nCurrent Board:\n");
    
    sprintf(line, "   ");
    for (int j = 0; j < SIZE; j++) {
        sprintf(line + strlen(line), "%2d ", j);
    }
    sprintf(buffer + strlen(buffer), "%s\n", line);
    
    sprintf(line, "  +");
    for (int j = 0; j < SIZE; j++) {
        sprintf(line + strlen(line), "---");
    }
    sprintf(buffer + strlen(buffer), "%s\n", line);
    
    for (int i = 0; i < SIZE; i++) {
        sprintf(line, "%2d|", i);
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0)
                sprintf(line + strlen(line), " . ");
            else if (board[i][j] == 1)
                sprintf(line + strlen(line), " X ");
            else
                sprintf(line + strlen(line), " O ");
        }
        sprintf(buffer + strlen(buffer), "%s\n", line);
    }
    
    Rio_writen(fd, buffer, strlen(buffer));
}