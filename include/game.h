#ifndef GAME
#define GAME

#include <stdlib.h>
#include "util.h"

typedef struct Morpion{
    int size;
    int* board;
} Morpion;

Morpion* newMorpion();

void drawMorpion(Morpion* m);

#endif /* GAME */
