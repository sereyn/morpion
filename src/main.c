#include <stdlib.h>

#include "../include/game.h"

int main(){
    Morpion* m = newMorpion();
    int winner = EMPTY;

    drawMorpion(m);

    while(winner == EMPTY){
        waitPlayer(m);
        printf("Your move:\n");
        drawMorpion(m);

        winner = checkWinner(m);
        if(winner != EMPTY) break;

        play(m);
        printf("Computer move:\n");
        drawMorpion(m);

        winner = checkWinner(m);
    }

    if(winner == COMPUTER) printf("Computer win!\n");
    else if(winner == PLAYER) printf("You win!\n");
    else printf("Draw\n");

    printf("%d\n", winner);

    exit(EXIT_FAILURE);
}

