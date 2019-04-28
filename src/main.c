#include <stdlib.h>

#include "../include/game.h"

int main(){
    Morpion* m = newMorpion();

    drawMorpion(m);

    while(!checkWinner(m)){
        waitPlayer(m);
        drawMorpion(m);
        checkWinner(m);
        play(m);
        drawMorpion(m);
    }

    exit(EXIT_FAILURE);
}

