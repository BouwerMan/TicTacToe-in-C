#include "bitboard.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
#define TIE 3
// TODO: More defines for board masks and shifts

const int winConditions[8] = {0b111, 0b111000, 0b111000000,
                              0b100100100, 0b010010010, 0b001001001,
                              0b100010001, 0b001010100};


const int boardOneMask = 0b00000000000000000000000111111111;
const int boardTwoMask = 0b00000001111111110000000000000000;
const int boardsMask   = boardOneMask | boardTwoMask;
const int playersMask =  0b01100000000000000000000000000000;
const int boardShift = 16;
const int playerShift = 29;

const char D_BOARD_S[3][3][1] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};


// Lmao literally wtf is this
// Again assumes 3x3 board with 2 players only
void printBoard(int bb) {
    char ch[3];
    int moveRaw, move1, move2;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            moveRaw = (bb >> 8 - ((i*3)+j));
            move1 = moveRaw & 1;
            move2 = (moveRaw >> boardShift) & 1;
            if (move1 != 0)
            {
                ch[j] = 'X';
            } 
            else if (move2 != 0)
            {
                ch[j] = 'O';
            }
            else
            {
                ch[j] = D_BOARD_S[i][j][0];
            }
        }

        printf("  %c  |  %c  |  %c  \n", ch[0], ch[1], ch[2]);

        if (i < 2) printf("-----------------\n");
    }
}

int getAvailableMoves(int board) {
    return (~board) & boardsMask;
}

int isMoveValid(int *board, int move) {
    int boardOne = (*board) & boardOneMask;
    int boardTwo = ((*board) & boardTwoMask) >> boardShift;
    int moves = boardOne | boardTwo;
    // Moves depending on who played it.
    if (move > 0x8000) { move = move >> boardShift; }
    return (move & ~moves) ? 1 : 0;
}

// Returns success value
int makeMove(int *board, int move) {
    int valid = isMoveValid(board, move);
    if (!valid) { return 0; }
    *board = *board + move;
    return 1;
}

int checkForWin(int board) {
    int trimmedBoard;
    int isWinner;

    int boards[2] = { board & boardOneMask,
                     (board & boardTwoMask) >> boardShift };
    
    for (size_t b = 0; b < 2; b++) {
        for (size_t win = 0; win < 8; win++) {
            trimmedBoard = boards[b] & winConditions[win];
            isWinner = trimmedBoard == winConditions[win];
            
            if (isWinner) { return b + 1; }
        }
    }

    if (!getAvailableMoves(board)) { return TIE; }

    return 0;
}

/** Mainly used for tests. Translates board square 1-9 to bitboard move */
int parseInput(int input, int player) {
    int move = 1 << (8 - (input - 1));
    return move << (boardShift * (player - 1));
}

int getPlayer(int* restrict board) {
    int playerBits = (*board) & playersMask;
    return playerBits >> playerShift;
}

void setPlayer(int* board, int player) {
    // Effectively sets both player bits to 0 allowing for replacement
    int playerBits = (*board) & ~playersMask;

    // Shifts player bits to left and uses mask to trim extra bits
    int newPlayer = (player << playerShift) & playersMask;

    (*board) = playerBits | newPlayer;
}