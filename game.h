#ifndef GAME_H
#define GAME_H

#define SIZE 20

int check_winner(int board[SIZE][SIZE], int row, int col);
void handle_game(int player1_fd, int player2_fd);
void send_board(int fd, int board[SIZE][SIZE]);

#endif