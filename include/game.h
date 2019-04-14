#ifndef GAME
#define GAME

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "util.h"

#define EMPTY '.'
#define PLAYER 'X'
#define COMPUTER 'O'

typedef struct Morpion{
    int size, winCondition;
    int* board;
} Morpion;

Morpion* newMorpion();

void drawMorpion(Morpion* m);

void waitPlayer(Morpion* m);

void play(Morpion* m);

int checkSerie(Morpion* m, int p);

int checkWinner(Morpion* m);

#endif /* GAME */
