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

    m->size = 10;
    m->winCondition = 5;
    m->depthLimit = 3;

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
    int maxIndex = 0;
    Tree* t;
    
    if(m->depthLimit == 0){ /* random mode */
        for(i = 0; i < m->size * m->size; i++)
            if(m->board[i] == EMPTY)
                freePos[nbr++] = i;
        
        m->board[freePos[rand() % nbr]] = COMPUTER;
    }else{ /* minmax mode */
        t = getCurrentTree(m);

        for(i = 1; i < t->nChildren; i++){
            if(t->children[i]->value > t->children[maxIndex]->value)
                maxIndex = i;
        }
        m->board[t->children[maxIndex]->boardIndex] = COMPUTER;
    }
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

int _evalValue(int* board, int size, int toWin){
    int v = 0;
    int i;
    float angle;
    int current, moveX, moveY;
    int enemy;
    int lineScore;
    int lineLength;

    for(i = 0; i < size * size; i++){
        if(board[i] != EMPTY){
            /* enemy setup */
            if(board[i] == PLAYER)
                enemy = COMPUTER;
            else
                enemy = PLAYER;
            for(angle = 0; angle < 7 * (M_PI / 4); angle += M_PI / 4){ /* for each angle */
                current = i;
                lineScore = 0;
                lineLength = 1;
                moveX = _signErr(cos(angle), 0.1);
                moveY = _signErr(sin(angle), 0.1); 
                while(lineLength <= toWin){
                    if(board[current] == PLAYER){
                        lineScore -= SCORE_FILLED * lineLength * SEQ_FACTOR;
                    }else if(board[current] == COMPUTER){
                        lineScore += SCORE_FILLED * lineLength * SEQ_FACTOR;
                    }
                    /* check bounds */
                    if((current % size) + moveX >= size || (current % size) + moveX < 0){ /* x out */
                        break;
                    }else if(current + moveX + moveY * size >= size * size || current + moveX + moveY * size < 0){
                        break;
                    }
                    /* change pos */
                    current += moveX + moveY * size;
                    if(board[current] == enemy) break;
                    lineLength++;
                }
                if(lineLength >= toWin){
                    v += lineScore;   
                }
            }
        }
    }
    
    return v;
}

void debugBoard(int* board, int size){
    int i, j, hashed;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            hashed = i * size + j;
            printf("%c ", board[hashed]);
        }
        printf("\n");
    }
    printf("\n");
}

Tree* _getTreeRec(int* board, int size, int depth, int toWin, int player, int boardIndex){
    Tree* t; 
    int i, nextPlayer, better;
    int* tmpBoard; 


    t = mylloc(sizeof(Tree));

    t->boardIndex = boardIndex;
    
    /* depth check */
    if(depth <= 0){
        t->value = _evalValue(board, size, toWin);
        t->nChildren = 0;
        t->children = NULL;

        return t;
    }
    
    *t = (Tree){0, mylloc(size*size*sizeof(Tree*)), 0, boardIndex};
    
    tmpBoard = mylloc(size*size*sizeof(int));

    /* next player */
    if(player == COMPUTER){
        nextPlayer = PLAYER;
    }else{
        nextPlayer = COMPUTER;
    }
    /* search for empty positions (each child) */
    for(i = 0; i < size*size; i++){
        if(board[i] == EMPTY){
            t->nChildren++;
            /* copy board for the recursion */
            memcpy(tmpBoard, board, size*size*sizeof(int));
            /* apply the hypothetic choice */
            tmpBoard[i] = player;

            /* recursion */
            t->children[t->nChildren - 1] = _getTreeRec(tmpBoard, size, depth-1, toWin, nextPlayer, i);
        }
    }

    /* minmax */
    better = 0;
    for(i = 0; i < t->nChildren; i++){
        if(player == COMPUTER){ /* max */
            if(t->children[better]->value < t->children[i]->value)
                better = i;
        }else{ /* min */
            if(t->children[better]->value > t->children[i]->value)
                better = i;
        }
    }
    
    if(t->nChildren > 0)
        t->value = t->children[better]->value;

    return t; 
}

Tree* getCurrentTree(Morpion* m){
    return _getTreeRec(m->board, m->size, m->depthLimit, m->winCondition, COMPUTER, -1);
}

