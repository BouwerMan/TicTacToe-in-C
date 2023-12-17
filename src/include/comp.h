#include <stdbool.h>

#ifndef COMP_H_
#define COMP_H_

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; })

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a < _b ? _a : _b; })

extern int computerLevel;
extern int computerNum;

int genMove(const int bb);
int findBestMove(int bb);
int miniMax(int bb, int depth, bool isMax);
int eval(const int bb);
int genRandomMove(const int bb);

#endif /** COMP_H_ */