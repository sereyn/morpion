#include "../include/util.h"

void* mylloc(size_t s){
    void* p = malloc(s);
    if(p == NULL){
        printf("mylloc: allocation impossible\n");
        exit(EXIT_FAILURE);
    }
    return p;
}
