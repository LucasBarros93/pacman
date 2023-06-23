#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Funcao usleep
#include <curses.h>   // Funcoes de tela com clear (clrscr)
#include <poll.h>

#define SPEED  100000
#define RESETCOLOR "\x1B[0m"

#define ROWS  31
#define COLS  28

char tab[ROWS][COLS];

// typedef struct{
//     int x;
//     int y;
//     char dir;
// } Pacman;

// Pacman pac;

// typedef struct{
//     int x;
//     int y;
//     char dir;
// } Ghost;


// Ghost clyde;


// pac.x = 14;
// pac.y = 23;
// pac.dir = 's';


int posX = 14;
int posY = 23;
char direction = 's';

struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };

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
                printf("\x1b[38;5;19m" "⣿");
                continue;
            }


            if(tab[i][j] == '.'){
                printf(RESETCOLOR "·");
                continue;
            }
            if(tab[i][j] == 'o'){
                printf(RESETCOLOR "•");
                continue;
            }


            if(tab[i][j] == 'D'){ //&& direction =='d'){
                printf("\x1b[38;5;11m" "ᗧ");
                continue;
            }
            if(tab[i][j] == 'A'){ //&& direction =='a'){
                printf("\x1b[38;5;11m" "ᗤ");
                continue;
            }
            if(tab[i][j] == 'S'){ //&& direction =='s'){
                printf("\x1b[38;5;11m" "ᗣ");
                continue;
            }
            if(tab[i][j] == 'W'){ //&& direction =='w'){
                printf("\x1b[38;5;11m" "ᗢ");
                continue;
            }


            if(tab[i][j] == 'C'){
                printf("\x1b[38;5;214m" "ᗝ");
                continue;
            }
            if(tab[i][j] == 'B'){
                printf("\x1b[38;5;197m" "ᗝ");
                continue;
            }
            if(tab[i][j] == 'P'){
                printf("\x1b[38;5;213m" "ᗝ");
                continue;
            }
            if(tab[i][j] == 'I'){
                printf("\x1b[38;5;81m" "ᗝ");
                continue;
            }

            printf("%c", tab[i][j]);
        }

        printf("\n\r");
    }
}

void move_pacman(void){
    if(direction == 'd'){
        if(posX+1 > COLS-1){
            tab[posY][posX] = ' ';
            posX = 0;
            tab[posY][posX] = 'D';
        }

        else if(tab[posY][posX+1] != '#'){
            tab[posY][posX] = ' ';
            posX += 1;
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

    curs_set (0);  // Hide Cursor 

    initscr();

    //NÃO TO ENTENDENDO LEGAL
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    //ATÉ AQUI

    noecho();

    clear();
    refresh();

    init_tab();
    print_tab();

    while(1){

        //refresh();    NÃO USAR SE NÃO SEBE COMO
        //usleep(SPEED);

        if(poll(&mypoll, 1, 200)){
            clear();
            direction = getch();
            move_pacman();
        }
        else{
            clear();
            move_pacman();
        }

        //direction = getch();
        //move_pacman();

        print_tab();
        clear();
    }

    endwin(); //olá mundo
}