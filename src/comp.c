#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "comp.h"
#include "bitboard.h"

#define RANGE 8
#define MIN 0

int computerLevel = 0;
int computerNum = 2;

int genMove(const int bb) {
    int move = 1;
    if (computerLevel == 0) {
        move = genRandomMove(bb);
    }
    return move;
}

int genRandomMove(const int bb) {
    int move, r;
    int shift = boardShift * (computerNum - 1);
    do
    {
        r = random() % RANGE + MIN;
        move = (1 << r) << shift;
    } while (!isMoveValid(bb, move));
    return move;
}