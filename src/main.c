#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "bitboard.h"
#include "comp.h"

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

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

/** Gets user move and converts it to bitboard format. */
int getUserMove(int player) {
    // Buffer of 2 keeps it at 1 digit for ease + /0
    char buff[2];
    int st, moveRaw, move;
    char prmpt[] = "Player X, Please select your move: ";
    prmpt[7] = player + '0';
    int validInput = 0;
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
            } else if ((buff[0] == 'q') || (buff[0] == 'Q')) {
                // Checks for quit command.
                return -1;
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
    int bb = 0b10100000000000001000000000000000;
    int move;
    // makeMove(&bb, 0b10 << boardShift);
    // makeMove(&bb, 0b10000 << boardShift);
    // makeMove(&bb, 0b10000000 << boardShift);

    int player, newPlayer;

    while (1) {
        printBoard(bb);
        player = getPlayer(bb);
        if (player == 2) {
            move = genMove(bb);
        } else {
            move = getUserMove(player);
        }
        if (move == -1) { break; }
        if (!isMoveValid(bb, move)) {
            printf("Invalid move.\n");
            continue;
        }
        (void) makeMove(&bb, move);

        if (checkForWin(bb) != 0) {
            printBoard(bb);
            printf("Player %d won!\n", player);
            break;
        }

        // Iterating turn
        newPlayer = ~player & 0b11;
        setPlayer(&bb, newPlayer);
    }

    return 0;
}