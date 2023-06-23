#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Funcao usleep
#include <curses.h>   // Funcoes de tela com clear (clrscr)

#define SPEED  100000
#define RESETCOLOR "\x1B[0m"

#define ROWS  31
#define COLS  28  //TEM QUE TA VENDO ISSO AQUI, FAZER JUNTO COM A V2 DA init_tab()

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
    rewind( fp );

    int i = 0;
    while (fgets(tab[i], 30, fp)){
        i++;
    }


    fclose(fp);
}


// usar ⡇⢸ um dia pra saida dos fantasmas 
void print_tab(void){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            if(tab[i][j] == '#'){
                printf("\x1b[38;5;19m" "⣿" RESETCOLOR);
                continue;
            }


            if(tab[i][j] == '.'){
                printf("·" RESETCOLOR);
                continue;
            }
            if(tab[i][j] == 'o'){
                printf("•" RESETCOLOR);
                continue;
            }


            if(tab[i][j] == 'D'){ //&& direction =='d'){
                printf("\x1b[38;5;11m" "ᗧ" RESETCOLOR);
                continue;
            }
            if(tab[i][j] == 'A'){ //&& direction =='a'){
                printf("\x1b[38;5;11m" "ᗤ" RESETCOLOR);
                continue;
            }
            if(tab[i][j] == 'S'){ //&& direction =='s'){
                printf("\x1b[38;5;11m" "ᗣ" RESETCOLOR);
                continue;
            }
            if(tab[i][j] == 'W'){ //&& direction =='w'){
                printf("\x1b[38;5;11m" "ᗢ" RESETCOLOR);
                continue;
            }


            if(tab[i][j] == 'C'){
                printf("\x1b[38;5;214m" "ᗝ" RESETCOLOR);
                continue;
            }
            if(tab[i][j] == 'B'){
                printf("\x1b[38;5;197m" "ᗝ" RESETCOLOR);
                continue;
            }
            if(tab[i][j] == 'P'){
                printf("\x1b[38;5;213m" "ᗝ" RESETCOLOR);
                continue;
            }
            if(tab[i][j] == 'I'){
                printf("\x1b[38;5;81m" "ᗝ" RESETCOLOR);
                continue;
            }

            printf("%c", tab[i][j]);
        }

        printf("\n\r");
    }
}

void move_pacman(){
    if(direction == 'd'){
        if(posX+1 > COLS-1){
            tab[posY][posX] = ' ';
            posX = 0;
            tab[posY][posX] = 'D';
        }

        else if(tab[posY][posX+1] != '#'){
            tab[posY][posX] = ' ';
            
            posX += 1;
            printf("%d", posX);

            tab[posY][posX] = 'D';
        }
    }

    if(direction == 'a'){
        if(posX-1 < 0){
            tab[posY][posX] = ' ';
            posX = COLS-1;
            tab[posY][posX] = 'A';
        }

        else if(tab[posY][posX-1] != '#'){
            tab[posY][posX] = ' ';
            
            posX -= 1;
            printf("%d", posX);

            tab[posY][posX] = 'A';
        }
    }

    if(direction == 's'){

        if(tab[posY+1][posX] != '#'){
            tab[posY][posX] = ' ';
            
            posY += 1;
            tab[posY][posX] = 'S';
        }
    }

    if(direction == 'w'){

        if(tab[posY-1][posX] != '#'){
            tab[posY][posX] = ' ';
            
            posY -= 1;
            tab[posY][posX] = 'W';
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