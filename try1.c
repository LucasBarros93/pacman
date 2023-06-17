#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Funcao usleep
#include <curses.h>   // Funcoes de tela com clear (clrscr)

#define SPEED  10000

#define ROWS  15
#define COLS  50

char tab[ROWS][COLS];

void init_tab(void){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){

            if(i == 0 || i == ROWS-1){
                tab[i][j] = '#';
            }
            else if(j == 0 || j == COLS-1){
                tab[i][j] = '#';
            }
            else{
                tab[i][j] = '.';
            }
        }
    }
}

void print_tab(void){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            printf("%c", tab[i][j]);
        }

        printf("\n");
    }
}

void main(void){
    init_tab();
    print_tab();
}