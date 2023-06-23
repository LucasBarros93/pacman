//
// Demo: Animacao controlada pelo teclado
//       Movendo uma "nave" pelo teclado (a,s,d,w)
// 
// By FOsorio 2023
// LINUX Version
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Funcao usleep
#include <curses.h>   // Funcoes de tela com clear (clrscr)

#define SPEED  10000

int Cols;
int Lins;

void clrscr(void)
{
     clear();
     refresh();
}

int main()
{
    int i;
    char tecla;

    Cols=0;
    Lins=0;

    //NÃO COMPREENDO NADA
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set (0);  // Hide Cursor 
    clrscr();
    //ATE AQUI

    while (true) 
    {
        for (i=0; i <= Lins; i++)
        {
            printf("\n\r");
        }
        for (i=0; i <= Cols; i++)
        {
            printf(" ");
        }
        printf("(*)\n\r");   // Importante colocar o \n\r após desenho da linha

        usleep(SPEED);
        clrscr();

        tecla = getch();
        if (tecla == 'x')
          break;
        if (tecla == 'w')    // Up
          Lins--;
        if (tecla == 's')    // Down
          Lins++;
        if (tecla == 'a')    // Left
          Cols--;
        if (tecla == 'd')    // Right
          Cols++;

    }
    endwin();
    return 0; 
}

