#ifndef BITBOARD_H_
#define BITBOARD_H_

#define TIE 3

extern const int winConditions[8];
extern const int boardOneMask;
extern const int boardTwoMask;
extern const int boardsMask;
extern const int playersMask;
extern const int boardShift;
extern const int playerShift;

extern const char D_BOARD_S[3][3];

/** Prints Board (bb) */
void printBoard(const int board);

int getAvailableMoves(const int board);

/** Checks if move is valid */
int isMoveValid(const int board, int move);

/** Playes move in board, returns success code */
int makeMove(int *board, const int move);

//TODO: Make this modify gamestate?
/** Checks for win in board */
int checkForWin(const int board);

/** Mainly used for tests. Translates board square 1-9 to bitboard move */
int parseInput(const int input, const int player);

int getPlayer(const int board);

void setPlayer(int* restrict board, int player);

#endif /** BITBOARD */