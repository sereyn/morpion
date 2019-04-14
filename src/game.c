#include "../include/game.h"

int _signErr(float a, float err){
    if(a < -err){
        return -1;
    }else if(a > err){
        return 1;
    }
    return 0;
}

int _logWeird(int a){
    if(a == 0)
        return 0; 
    return (int)log10(a);
}

Morpion* newMorpion(){
    Morpion* m = mylloc(sizeof(Morpion));
    int i;

    m->size = 3;
    m->winCondition = 3;

    m->board = mylloc(m->size * m->size * sizeof(int));
    for(i = 0; i < m->size * m->size; i++){
        m->board[i] = EMPTY;
    }

    srand(time(NULL));

    return m;
}

void drawMorpion(Morpion* m){
    int i, j, hashed;

    /* top line */
    for(i = 0; i < _logWeird(m->size) + 2; i++){
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
        for(j = 0; j < _logWeird(m->size) - _logWeird(i); j++)
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
    for(i = 0; i < _logWeird(m->size) + 2; i++)
        printf(" "); /* offset */
    printf("+");
    for(i = 0; i <= m->size * 2; i++)
        printf("-");
    printf("+\n ");
    /* bottom letters */
    for(i = 0; i < _logWeird(m->size) + 2; i++)
        printf(" "); /* offset */
    for(i = 0; i < m->size; i++)
        printf(" %c", 'a' + i);
    printf("\n");
}

void waitPlayer(Morpion* m){
    int a; char b; int hashed;
    int good = 0;
    
    while(!good){ 
        scanf("%d%c", &a, &b);
        /* check bounds */
        if(!(a < 0 || a >= m->size || b - 'a' < 0 || b - 'a' >= m->size)){
            /* check if empty (good) */
            hashed = a * m->size + b - 'a';
            if(m->board[hashed] == EMPTY){
                m->board[hashed] =  PLAYER;    
                good = 1;
            }else{
                printf("already occupated\n");
            }
        }else{
            printf("out of bound\n");
        }
    }
}

void play(Morpion* m){
    int* freePos = mylloc(m->size * m->size * sizeof(int));
    int i, nbr = 0;
    
    for(i = 0; i < m->size * m->size; i++)
        if(m->board[i] == EMPTY)
            freePos[nbr++] = i;
            
    m->board[freePos[rand() % nbr]] = COMPUTER;

    free(freePos);
}

int checkSerie(Morpion* m, int p){
    float i;
    int nbr;
    int current;
    int moveX, moveY;

    if(m->board[p] == EMPTY)
        return 0;

    for(i = 0; i < 7 * (M_PI / 4); i += M_PI / 4){ /* for each angle */
        nbr = 0;
        current = p;
        /* compute next move */
        moveX = _signErr(cos(i), 0.1);
        moveY = _signErr(sin(i), 0.1);
        while(m->board[current] == m->board[p]){
            nbr++;
            /* check bounds */
            if((current % m->size) + moveX >= m->size || (current % m->size) + moveX < 0){ /* x out */
                break;
            }else if(current + moveX + moveY * m->size >= m->size * m->size || current + moveX + moveY * m->size < 0){
                break;
            }
            /* change pos */
            current += moveX + moveY * m->size;
        }
        if(nbr >= m->winCondition){
            return m->board[p];
        }
    }
    return 0;
}

int checkWinner(Morpion* m){
    int i;
    int winner;
    for(i = 0; i < m->size * m->size; i++){
        winner = checkSerie(m, i);
        if(winner == PLAYER){
            printf("You win!\n");
            return PLAYER;
            break;
        }else if(winner == COMPUTER){
            printf("Computer win!\n");
            return COMPUTER;
            break;
        }
    }
    return 0;
}

