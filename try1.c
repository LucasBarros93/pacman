#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Funcao usleep
#include <curses.h>   // Funcoes de tela com clear (clrscr)

#define SPEED  100000

#define ROWS  31
#define COLS  29  //TEM QUE TA VENDO ISSO AQUI, FAZER JUNTO COM A V2 DA init_tab()

char tab[ROWS][COLS];

int posX = 14;
int posY = 23;

char direction = 's';

void init_tab(void){
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( "map.txt" , "rb" );
    if( !fp ) perror("map.txt"),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
    fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    /* do your work here, buffer is a string contains the whole text */

    int j=0, k=0;

    for(int i=0; i<lSize; i++){
        if(buffer[i] == '\n'){
            tab[j][k] = ' ';
            j++;
            k=0;
            continue;
        }
        tab[j][k] = buffer[i];
        k++;
    }

    fclose(fp);
    free(buffer);
}


// usar ⣿ um dia pra parede, ᗣ pra fantasma, ᗧ ᗤ pro pacman, ·····•····· pros pontinhos
void print_tab(void){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            printf("%c", tab[i][j]);
        }

        printf("\n\r");
    }
}

void move_pacman(){
    if(direction == 'd'){
        if(tab[posY][posX+1] != '#'){
            tab[posY][posX] = ' ';
            
            posX += 1;
            if(posX > COLS-2) posX = 0;

            tab[posY][posX] = 'C';
        }
    }

    if(direction == 'a'){

        if(tab[posY][posX-1] != '#'){
            tab[posY][posX] = ' ';
            
            posX -= 1;
            if(posX < 0) posX = COLS-2; //TEM QUE AJUSTAR O NUMERO DE COLUNAS

            tab[posY][posX] = 'C';
        }
    }

    if(direction == 's'){

        if(tab[posY+1][posX] != '#'){
            tab[posY][posX] = ' ';
            
            posY += 1;
            tab[posY][posX] = 'C';
        }
    }

    if(direction == 'w'){

        if(tab[posY-1][posX] != '#'){
            tab[posY][posX] = ' ';
            
            posY -= 1;
            tab[posY][posX] = 'C';
        }
    }
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

    while(1){

        clear();
        //refresh();
        usleep(SPEED);

        direction = getch();

        move_pacman();
        print_tab();
    }

    endwin();
}