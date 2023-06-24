#include <stdio.h>
#include <stdlib.h>

void main(void){
    int r;

    srand(time(NULL));

    char directions[4] = {'s','w','d','a'};
    r = directions[rand()%1];

    printf("%c", r);
}