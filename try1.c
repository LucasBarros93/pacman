#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Funcao usleep
#include <curses.h>   // Funcoes de tela com clear (clrscr)

#define SPEED  100000

#define ROWS  10
#define COLS  50

char tab[ROWS][COLS];

int posX = 1;
int posY = 1;

void init_tab(void){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            tab[i][j] = '.';

            if(i == 0 || i == ROWS-1){
                tab[i][j] = '#';
            }
            if(j == 0 || j == COLS-1){
                tab[i][j] = '#';
            }

            if(i == posY && j == posX){
                tab[i][j] = 'C';
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

void move_pacman(){
    tab[posY][posX] = '.';
    
    posX += 1;
    tab[posY][posX] = 'C';
}

void main(void){

    //NÃO TO ENTENDENDO LEGAL
    curs_set (0);  // Hide Cursor 

    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();

    clear();
    refresh();
    //ATÉ AQUI

    init_tab();
    print_tab();

    while(posX < COLS){

        clear();
        //refresh();
        usleep(SPEED);

        move_pacman();
        print_tab();
    }

    endwin();
}