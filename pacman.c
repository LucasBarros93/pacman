#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Funcao usleep
#include <curses.h>   // Funcoes de tela com clear (clrscr)
#include <poll.h>
#include <time.h>

#define SPEED  100000
#define RESETCOLOR "\x1B[0m"

#define ROWS  31
#define COLS  28

char tab[ROWS][COLS];
char keyPressed = ' ';

//CRIANDO E INICIANDO O PACMAN COMO UM STRUCT
typedef struct{
    int x;
    int y;
    char dir;
} Pacman;

Pacman pac = {14, 23, 'a'};

//CRIANDO E INICIANDO FANTASMAS
typedef struct{
    int x;
    int y;
    char dir;
    int inGame;
    char under;
} Ghost;

Ghost clyde = {15, 11, 'a', 1, ' '};

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


            if(tab[i][j] == 'A' && pac.dir =='d'){
                printf("\x1b[38;5;11m" "ᗧ");
                continue;
            }
            if(tab[i][j] == 'A' && pac.dir =='a'){
                printf("\x1b[38;5;11m" "ᗤ");
                continue;
            }
            if(tab[i][j] == 'A' && pac.dir =='s'){
                printf("\x1b[38;5;11m" "ᗣ");
                continue;
            }
            if(tab[i][j] == 'A' && pac.dir =='w'){
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
    if(keyPressed == 'd'){
        if(pac.x+1 > COLS-1){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';
            pac.x = 0;
            tab[pac.y][pac.x] = 'A';
        }

        else if(tab[pac.y][pac.x+1] != '#'){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';
            pac.x += 1;
            tab[pac.y][pac.x] = 'A';
        }
        else{
            keyPressed = pac.dir;
        }
    }

    if(keyPressed == 'a'){
        if(pac.x-1 < 0){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';
            pac.x = COLS-1;
            tab[pac.y][pac.x] = 'A';
        }

        else if(tab[pac.y][pac.x-1] != '#'){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';
            pac.x -= 1;
            tab[pac.y][pac.x] = 'A';
        }
        else{
            keyPressed = pac.dir;
        }
    }

    if(keyPressed == 's'){

        if(tab[pac.y+1][pac.x] != '#'){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';          
            pac.y += 1;
            tab[pac.y][pac.x] = 'A';
        }
        else{
            keyPressed = pac.dir;
        }
    }

    if(keyPressed == 'w'){

        if(tab[pac.y-1][pac.x] != '#'){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';
            pac.y -= 1;
            tab[pac.y][pac.x] = 'A';
        }
        else{
            keyPressed = pac.dir;
        }
    }
}

void move_ghost(void){
    int options = 0;
    char directions[4];
    srand(time(NULL));

    if(clyde.dir == 'd'|| clyde.dir == 'a'){
        if(tab[clyde.y+1][clyde.x] != '#' || tab[clyde.y-1][clyde.x] != '#'){
            if (tab[clyde.y+1][clyde.x] != '#') {
                directions[options] = 's';
                options++;
            }
            if (tab[clyde.y-1][clyde.x] != '#') {
                directions[options] = 'w';
                options++;
            }
            if (tab[clyde.y][clyde.x+1] != '#') {
                directions[options] = 'd';
                options++;
            }
            if (tab[clyde.y][clyde.x-1] != '#') {
                directions[options] = 'a';
                options++;
            }

            clyde.dir = directions[rand()%(options)];
        }
    }

    else{
        if(tab[clyde.y][clyde.x+1] != '#' || tab[clyde.y][clyde.x-1] != '#'){
            if (tab[clyde.y+1][clyde.x] != '#') {
                directions[options] = 's';
                options++;
            }
            if (tab[clyde.y-1][clyde.x] != '#') {
                directions[options] = 'w';
                options++;
            }
            if (tab[clyde.y][clyde.x+1] != '#') {
                directions[options] = 'd';
                options++;
            }
            if (tab[clyde.y][clyde.x-1] != '#') {
                directions[options] = 'a';
                options++;
            }

            clyde.dir = directions[rand()%(options)];
        }
    }

    if(clyde.dir == 'd'){
        if(clyde.x+1 > COLS-1){
            tab[clyde.y][clyde.x] = clyde.under;
            clyde.x = 0;
            clyde.under = tab[clyde.y][clyde.x];
            tab[clyde.y][clyde.x] = 'C';
        }

        else{
            tab[clyde.y][clyde.x] = clyde.under;
            clyde.x += 1;
            clyde.under = tab[clyde.y][clyde.x];
            tab[clyde.y][clyde.x] = 'C';
        }
    }
    if(clyde.dir == 'a'){
        if(clyde.x-1 < 0){
            tab[clyde.y][clyde.x] = clyde.under;
            clyde.x = COLS-1;
            clyde.under = tab[clyde.y][clyde.x];
            tab[clyde.y][clyde.x] = 'C';
        }

        else{
            tab[clyde.y][clyde.x] = clyde.under;
            clyde.x -= 1;
            clyde.under = tab[clyde.y][clyde.x];
            tab[clyde.y][clyde.x] = 'C';
        }
    }

    if(clyde.dir == 's'){
        tab[clyde.y][clyde.x] = clyde.under;          
        clyde.y += 1;
        clyde.under = tab[clyde.y][clyde.x];
        tab[clyde.y][clyde.x] = 'C';
    }
    if(clyde.dir == 'w'){
        tab[clyde.y][clyde.x] = clyde.under;          
        clyde.y -= 1;
        clyde.under = tab[clyde.y][clyde.x];
        tab[clyde.y][clyde.x] = 'C';
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
            keyPressed = getch();
            move_pacman();
            move_ghost();
        }
        else{
            clear();
            move_pacman();
            move_ghost();
        }

        //printf("%c", clyde.dir);
        //pac.dir = getch();
        //move_pacman();

        print_tab();
        clear();
    }

    endwin(); //olá mundo
}