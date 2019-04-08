#include "../include/game.h"

Morpion* newMorpion(){
    Morpion* m = mylloc(sizeof(Morpion));
    int i;

    m->size = 14;

    m->board = mylloc(m->size * m->size * sizeof(int));
    for(i = 0; i < m->size * m->size; i++){
        m->board[i] = '.';
    }

    return m;
}

void drawMorpion(Morpion* m){
    int i, j, hashed;

    /* top line */
    for(i = 0; i < m->size / 10 + 2; i++){
        printf(" "); /* offset */
    }
    printf("+");
    for(i = 0; i <= m->size * 2; i++){
        printf("-");
    }
    printf("+\n");
    /* for each line */
    for(i = 0; i < m->size; i++){
        /* left line */
        printf("%d", i);
        for(j = 0; j < (m->size / 10) - (i / 10); j++)
            printf(" "); /* offset */
        printf(" |");
        /* for each column */
        for(j = 0; j < m->size; j++){
            hashed = i * m->size + j;
            printf(" %c", m->board[hashed]);
        }
        printf(" |\n");
    }
    /* bottom line */
    for(i = 0; i < m->size / 10 + 2; i++)
        printf(" "); /* offset */
    printf("+");
    for(i = 0; i <= m->size * 2; i++)
        printf("-");
    printf("+\n ");
    /* bottom letters */
    for(i = 0; i < m->size / 10 + 2; i++)
        printf(" "); /* offset */
    for(i = 0; i < m->size; i++)
        printf(" %c", 'A' + i);
    printf("\n");
}
