#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
// TODO: More defines for board masks and shifts

const char D_BOARD_S[3][3][1] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};

static int getLine(char *prmpt, char *buff, size_t sz)
{
    int ch, extra;

    // Print prompt
    if (prmpt != NULL)
    {
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
int get_player_two_computer()
{
    char *prmpt = "Is Player Two a computer? 1=yes, 0=no: ";
    char buff[2];
    int i;

    while (1)
    {
        (void) getLine(prmpt, buff, sizeof(buff));

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
void printBoard(uint32_t bb)
{
    char ch[3];
    int moveRaw, move1, move2;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            moveRaw = (bb >> 8 - ((i*3)+j));
            move1 = moveRaw & 1;
            move2 = (moveRaw >> 16) & 1;
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

int main()
{
    printf("\nWelcome to Tic Tac Toe in C!\n");

    //! Will mess with this later, want to make board work first.
    //int comp = get_player_two_computer();
    //printf("%d\n", comp);
    uint32_t bb = 0b10100000000010001000000001000000;
    printBoard(bb);

    return 0;
}