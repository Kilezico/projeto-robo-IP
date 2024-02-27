#ifndef JOGO-DA-VELHA_H
#define JOGO-DA-VELHA_H

void velha();
char gridChar(int i);
void draw(int b[3][3]);
int win(const int board[3][3]);
int minimax(int board[3][3], int player);
void computerMove(int board[3][3]);
void playerMove(int board[3][3]);
#endif