#include <stdlib.h>

#include "../include/game.h"

int main(){
    Morpion* m = newMorpion();

    drawMorpion(m);

    while(1){
        waitPlayer(m);
        play(m);
        drawMorpion(m);
        if(checkWinner(m))
            break;
    }

    exit(EXIT_FAILURE);
}

