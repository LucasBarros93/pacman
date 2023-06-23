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
char keyPressed = 's';

//CRIANDO E INICIANDO O PACMAN COMO UM STRUCT
typedef struct{
    int x;
    int y;
    char dir;
} Pacman;

Pacman pac = {14, 23, 's'};

// typedef struct{
//     int x;
//     int y;
//     char dir;
// } Ghost;


// Ghost clyde;

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


            if(tab[i][j] == 'D'){ //&& pac.dir =='d'){
                printf("\x1b[38;5;11m" "ᗧ");
                continue;
            }
            if(tab[i][j] == 'A'){ //&& pac.dir =='a'){
                printf("\x1b[38;5;11m" "ᗤ");
                continue;
            }
            if(tab[i][j] == 'S'){ //&& pac.dir =='s'){
                printf("\x1b[38;5;11m" "ᗣ");
                continue;
            }
            if(tab[i][j] == 'W'){ //&& pac.dir =='w'){
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
    if(pac.dir == 'd'){
        if(pac.x+1 > COLS-1){
            tab[pac.y][pac.x] = ' ';
            pac.x = 0;
            tab[pac.y][pac.x] = 'D';
        }

        else if(tab[pac.y][pac.x+1] != '#'){
            tab[pac.y][pac.x] = ' ';
            pac.x += 1;
            tab[pac.y][pac.x] = 'D';
        }
    }

    if(pac.dir == 'a'){
        if(pac.x-1 < 0){
            tab[pac.y][pac.x] = ' ';
            pac.x = COLS-1;
            tab[pac.y][pac.x] = 'A';
        }

        else if(tab[pac.y][pac.x-1] != '#'){
            tab[pac.y][pac.x] = ' ';
            pac.x -= 1;
            tab[pac.y][pac.x] = 'A';
        }
    }

    if(pac.dir == 's'){

        if(tab[pac.y+1][pac.x] != '#'){
            tab[pac.y][pac.x] = ' ';
            
            pac.y += 1;
            tab[pac.y][pac.x] = 'S';
        }
    }

    if(pac.dir == 'w'){

        if(tab[pac.y-1][pac.x] != '#'){
            tab[pac.y][pac.x] = ' ';
            
            pac.y -= 1;
            tab[pac.y][pac.x] = 'W';
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
            pac.dir = getch();
            move_pacman();
        }
        else{
            clear();
            move_pacman();
        }

        //pac.dir = getch();
        //move_pacman();

        print_tab();
        clear();
    }

    endwin(); //olá mundo
}