#ifndef GAME
#define GAME

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "util.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define EMPTY '.'
#define PLAYER 'X'
#define COMPUTER 'O'

#define SCORE_FILLED 7
#define SEQ_FACTOR 4

typedef struct Tree{
    int value;
    struct Tree** children;
    int nChildren;
    int boardIndex;
} Tree;

typedef struct Morpion{
    int size, winCondition;
    int* board;
    int depthLimit;
} Morpion;

/* create a game object (a scene) */
Morpion* newMorpion();

void drawMorpion(Morpion* m);

/* wait the player's input and write it into the game object */
void waitPlayer(Morpion* m);

/* computer's turn to play */
void play(Morpion* m);

/* check if a position is the beginning of a winning line */
int checkSerie(Morpion* m, int p);

/* checkSeries of all positions */
int checkWinner(Morpion* m);

Tree* getCurrentTree(Morpion* m);

#endif /* GAME */
