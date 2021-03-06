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

    m->size = 9;
    m->winCondition = 5;
    m->depthLimit = 3;

    m->board = mylloc(m->size * m->size * sizeof(int));
    for(i = 0; i < m->size * m->size; i++){
        m->board[i] = EMPTY;
    }
    m->board[(m->size * m->size)/2] = COMPUTER;

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

int _isGood(int* board, int size, int a, int b){
    int good = 0;
    int hashed = a * size + b - 'a';

    /* check bounds */
    if(!(a < 0 || a >= size || b - 'a' < 0 || b - 'a' >= size)){
        /* already occupated */
        if(board[hashed] != EMPTY){
            return 0;
        }

        /* look around */
        if(b < 'a' + size - 1) good |= board[hashed + 1] != EMPTY;
        if(b < 'a' + size - 1 && a > 0) good |= board[hashed + 1 - size] != EMPTY;
        if(a > 0) good |= board[hashed - size] != EMPTY;
        if(a > 0 && b > 'a') good |= board[hashed - 1 - size] != EMPTY;
        if(b > 'a') good |= board[hashed - 1] != EMPTY;
        if(a < size - 1 && b > 'a') good |= board[hashed - 1 + size] != EMPTY;
        if(a < size - 1) good |= board[hashed + size] != EMPTY;
        if(a < size - 1 && b < 'a' + size - 1) good |= board[hashed + 1 + size] != EMPTY;
    }
    return good;
}

void waitPlayer(Morpion* m){
    int a; char b; int hashed;
    int good = 0;
    
    while(!good){ 
        scanf("%d%c", &a, &b);
        hashed = a * m->size + b - 'a';

        if(_isGood(m->board, m->size, a, b)){
            good = 1;
            m->board[hashed] = PLAYER;
        }
    }
}

void _freeTree(Tree** t){
    int i;
    for(i = 0; i < (*t)->nChildren; i++){
        _freeTree(&((*t)->children[i]));
    }
    free((*t)->children);
    free(*t);
}

void play(Morpion* m){
    int* freePos = mylloc(m->size * m->size * sizeof(int));
    int i, nbr = 0;
    int a, b;
    int maxIndex = 0;
    Tree* t;

    if(m->depthLimit == 0){ /* random mode */
        for(a = 0; a < m->size; a++){
            for(b = 'a'; b < 'a' + m->size; b++){
                if(_isGood(m->board, m->size, a, b)){
                    freePos[nbr++] = a * m->size + b - 'a';
                }
            }
        }
        if(nbr != 0)
            m->board[freePos[rand() % nbr]] = COMPUTER;
    }else{ /* minmax mode */
        t = getCurrentTree(m);

        for(i = 1; i < t->nChildren; i++){
            if(t->children[i]->value > t->children[maxIndex]->value)
                maxIndex = i;
        }
        if(t->nChildren != 0)
            m->board[t->children[maxIndex]->boardIndex] = COMPUTER;
        _freeTree(&t);
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

int _isFull(Morpion* m){
    int i;
    int full = 1;
    for(i = 0; i < m->size * m->size; i++){
        if(m->board[i] == EMPTY)
            full = 0;
    }
    return full;
}

int checkWinner(Morpion* m){
    int i;
    int winner;
    for(i = 0; i < m->size * m->size; i++){
        winner = checkSerie(m, i);
        if(winner == PLAYER){
            return PLAYER;
            break;
        }else if(winner == COMPUTER){
            return COMPUTER;
            break;
        }
    }
    if(_isFull(m)){
        return EMPTY;
    }
    return EMPTY;
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
                        if(lineLength >= toWin) return INT_MIN;
                        lineScore -= SCORE_FILLED * lineLength * SEQ_FACTOR;
                    }else if(board[current] == COMPUTER){
                        if(lineLength >= toWin) return INT_MAX;
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
    int a, b;
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
    /* search for playable positions (each child) */
    for(a = 0; a < size; a++){
        for(b = 'a'; b < 'a' + size; b++){
            if(_isGood(board, size, a, b)){
                t->nChildren++;
                /* copy board for the recursion */
                memcpy(tmpBoard, board, size*size*sizeof(int));
                /* apply the hypothetic choice */
                tmpBoard[a * size + b - 'a'] = player;

                /* debugBoard(tmpBoard, size); */

                /* recursion */
                t->children[t->nChildren - 1] = _getTreeRec(tmpBoard, size, depth-1, toWin, nextPlayer, a * size + b - 'a');
            }
        }
    }

    free(tmpBoard);

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

