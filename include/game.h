#ifndef GAME
#define GAME

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "util.h"

#define EMPTY '.'
#define PLAYER 'X'
#define COMPUTER 'O'

#define SCORE_EMPTY 1
#define SCORE_FILLED 4
#define SEQ_FACTOR 1

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
