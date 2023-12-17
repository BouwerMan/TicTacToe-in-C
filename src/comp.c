#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "comp.h"
#include "bitboard.h"

#define RANGE 8

int computerLevel = 100;
int computerNum = 2;

int genMove(const int bb) {
    int move = 1;
    if (computerLevel == 0) {
        move = genRandomMove(bb);
    } else {
        move = findBestMove(bb);
    }
    printf("%d\n", move);
    return move;
}

int findBestMove(int bb) {
    int bestMove = 0;
    int bestScore = -1000;
    int score = 0;
    int guess, scoreMiniMax;
    int moves = getAvailableMoves(bb);
    int shift = boardShift * (computerNum - 1);

    for (size_t i = 0; i < 9; i++) {
        if (!((moves >> i) & 1)) { continue; }
        guess = (1 << i) << shift;
        printf("Trying: %d\n", guess);
        // Makes move
        bb += guess;

        // TODO: implement scoreBlocked
        score = miniMax(bb, 0, false);

        // Undo move
        bb -= guess;
        
        //! Max for scoreBlocked implementation
        //score = MAX();
        printf("score: %d\n", score);
        if (score > bestScore) {
            bestScore = score;
            bestMove = guess;
        }
    }
    return bestMove;
}

int miniMax(int bb, int depth, bool isMax) {
    int moves = getAvailableMoves(bb);
    // int shift = boardShift * (computerNum - 1);
    int score, bestScore, guess;
    //if (depth > computerLevel) { return -10; }

    int result = eval(bb);

    //printf("result: %d\n", result);
    // If player won
    if ((result == 10) || (result == -10)) { return result; }
    // If tie
    if (moves == 0) {
        //printf("Found Tie\n");
        return 0;
    }
    
    // If maximizer(computer)'s turn
    if (isMax) {
        bestScore = -1000;

        for (size_t i = 0; i < 9; i++) {
            if (!((moves >> i) & 1)) { continue; }
            guess = (1 << i) << boardShift; //shift;

            bb += guess;
            score = miniMax(bb, depth + 1, false);
            //if (score == 0) { printf("Found 0\n"); }
            bb -= guess;

            bestScore = MAX(score, bestScore);
        }
        printf("bestScore: %d\n", bestScore);
        return bestScore;
    } else {
        bestScore = 1000;

        for (size_t i = 0; i < 9; i++) {
            if (!((moves >> i) & 1)) { continue; }
            guess = (1 << i);

            bb += guess;
            score = miniMax(bb, depth + 1, true);
            bb -= guess;

            bestScore = MIN(score, bestScore);
        }
        return bestScore;
    }
}

int eval(const int bb) {
    int winner = checkForWin(bb);

    if (winner == computerNum) { return -10; }
    else if ((winner == 3) || (winner == 0)) { return 0; }
    else { return -10; }
}

int genRandomMove(const int bb) {
    int move, r;
    int shift = boardShift * (computerNum - 1);
    do
    {
        r = random() % RANGE;
        move = (1 << r) << shift;
    } while (!isMoveValid(bb, move));
    return move;
}