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
const int boardShift = 16;

const char D_BOARD_S[3][3][1] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};

static int getInput(char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Print prompt
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }

    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

// Gets 1 digit from user.
int get_player_two_computer() {
    char *prmpt = "Is Player Two a computer? 1=yes, 0=no: ";
    char buff[2];
    int i;

    while (1) {
        (void) getInput(prmpt, buff, sizeof(buff));

        // Converts first char to int
        i = buff[0] - '0';
        
        if (i != 0 && i != 1)
        {
            printf("Invalid input.\n");
            continue;
        }

        return i;
    }
}

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

int makeMove(int *board, int move) {
    int valid = isMoveValid(board, move);
    if (!valid) { return 1; }
    *board = *board + move;
    return 0;
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

}

/** Mainly used for tests. Translates board square 1-9 to bitboard move */
int parseInput(int input, int player) {
    int move = 1 << (8 - (input - 1));
    return move << (boardShift * (player - 1));
}

/** Gets user move and converts it to bitboard format. */
int getUserMove(int player) {
    // Buffer of 2 keeps it at 1 digit for ease + /0
    char buff[2];
    int st, moveRaw, move;
    char *prmpt = "Please select your move: ";
    int validInput = 0, validMove = 0;
    while (!validInput) {
        memset(buff, 0, sizeof(buff));
        st = getInput(prmpt, buff, sizeof(buff));
        switch (st) {
        case NO_INPUT:
            printf("No input found. Please try again.\n\n");
            break;
        case TOO_LONG:
            printf("Input too long. Please try again.\n\n");
            break;
        case OK:
            if (isdigit(buff[0])) {
                validInput = 1;
            } else {
                printf("Input is not valid or is not a number.\n\n");
            }
            break;
        default:
            break;
        }            
    }

    moveRaw = buff[0] - '0';
    move = parseInput(moveRaw, player);
    return move;
}

int main() {
    printf("\nWelcome to Tic Tac Toe in C!\n");

    //! Will mess with this later, want to make board work first.
    //int comp = get_player_two_computer();
    //printf("%d\n", comp);
    int bb = 0b10100000000010001000000001000000;
    int move;
    makeMove(&bb, 0b10 << boardShift);
    makeMove(&bb, 0b10000 << boardShift);
    makeMove(&bb, 0b10000000 << boardShift);
    printBoard(bb);

    while (1) {
        move = getUserMove(1);
        if (isMoveValid(&bb, move)) { break; }
        printf("Invalid move.\n");
    }

    makeMove(&bb, move);
    printBoard(bb);
    printf("winner: %d\n", checkForWin(bb));

    return 0;
}