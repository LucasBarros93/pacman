#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Funcao usleep
#include <curses.h>   // Funcoes de tela com clear (clrscr)
#include <poll.h>
#include <time.h>

#define SPEED  500000
#define RESETCOLOR "\x1B[0m"

#define ROWS  31
#define COLS  28

char tab[ROWS][COLS];
char keyPressed = ' ';
int  gametime = 0;

//CRIANDO E INICIANDO O PACMAN COMO UM STRUCT
typedef struct{
    int x;
    int y;
    char dir;
} Pacman;

Pacman pac = {14, 23, 'a'};

//CRIANDO E INICIANDO FANTASMAS
typedef struct{
    char symbol;
    int x;
    int y;
    char dir;
    char under;
    int inGame;
    int dificult;
} Ghost;

Ghost clyde = {'C', 12, 14, 'a', ' ', 0, 0};
Ghost blynk = {'B', 13, 14, 'a', ' ', 0, 0};
Ghost pink  = {'P', 14, 14, 'a', ' ', 0, 0};
Ghost ink   = {'I', 15, 14, 'a', ' ', 0, 0};

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

void move_ghost(Ghost (*ghost)){
    if((*ghost).inGame){
        int options = 1;
        char directions[4];
        directions[0] = (*ghost).dir;
        srand(time(NULL));

        if((*ghost).dir == 'd'|| (*ghost).dir == 'a'){
            if(tab[(*ghost).y+1][(*ghost).x] != '#' || tab[(*ghost).y-1][(*ghost).x] != '#'){
                options = 0;
                if (tab[(*ghost).y+1][(*ghost).x] != '#') {
                    directions[options] = 's';
                    options++;
                }
                if (tab[(*ghost).y-1][(*ghost).x] != '#') {
                    directions[options] = 'w';
                    options++;
                }
                if (tab[(*ghost).y][(*ghost).x+1] != '#') {
                    directions[options] = 'd';
                    options++;
                }
                if (tab[(*ghost).y][(*ghost).x-1] != '#') {
                    directions[options] = 'a';
                    options++;
                }
            }
        }

        else{
            if(tab[(*ghost).y][(*ghost).x+1] != '#' || tab[(*ghost).y][(*ghost).x-1] != '#'){
                options = 0;
                if (tab[(*ghost).y+1][(*ghost).x] != '#') {
                    directions[options] = 's';
                    options++;
                }
                if (tab[(*ghost).y-1][(*ghost).x] != '#') {
                    directions[options] = 'w';
                    options++;
                }
                if (tab[(*ghost).y][(*ghost).x+1] != '#') {
                    directions[options] = 'd';
                    options++;
                }
                if (tab[(*ghost).y][(*ghost).x-1] != '#') {
                    directions[options] = 'a';
                    options++;
                }
            }
        }
        
        //VAMO ESCOLHER A DIREÇÃO DE FORMA INTELIGENTE AGR
        int finalOptions = 0;
        char finalDirections[4];

        if((*ghost).dificult){
            int goX = pac.x - (*ghost).x;
            int goY = pac.y - (*ghost).y;
            char idealDirectons[2];
            idealDirectons[0] = (goX < 0)? 'a':'d'; 
            idealDirectons[0] = (goX == 0)? ' ':idealDirectons[0]; 
            idealDirectons[1] = (goY > 0)? 's':'w';
            idealDirectons[1] = (goY == 0)? ' ':idealDirectons[1];

            for (int i = 0; i < options; i++) {
                int flag = 0;
                for (int j = 0; j < 2; j++) {
                    if (directions[i] == idealDirectons[j]) {
                        flag = 1;
                        break;
                    }
                }
                if (flag) {
                    finalDirections[finalOptions] = directions[i];
                    finalOptions++;
                }
            }
        }
        //SE ENTRAR NISSO AI DE CIMA O FANTASMA FICA DO MAU

        if(finalOptions == 0){
            (*ghost).dir = directions[rand()%(options)];
        }
        else{
            (*ghost).dir = finalDirections[rand()%(finalOptions)];
        }

        if((*ghost).dir == 'd'){
            if((*ghost).x+1 > COLS-1){
                tab[(*ghost).y][(*ghost).x] = (*ghost).under;
                (*ghost).x = 0;
                
                if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.'){
                    (*ghost).under = tab[(*ghost).y][(*ghost).x];
                }

                tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
            }

            else{
                tab[(*ghost).y][(*ghost).x] = (*ghost).under;
                (*ghost).x += 1;
                
                if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.'){
                    (*ghost).under = tab[(*ghost).y][(*ghost).x];
                }

                tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
            }
        }
        if((*ghost).dir == 'a'){
            if((*ghost).x-1 < 0){
                tab[(*ghost).y][(*ghost).x] = (*ghost).under;
                (*ghost).x = COLS-1;
                
                if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.'){
                    (*ghost).under = tab[(*ghost).y][(*ghost).x];
                }

                tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
            }

            else{
                tab[(*ghost).y][(*ghost).x] = (*ghost).under;
                (*ghost).x -= 1;
                
                if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.'){
                    (*ghost).under = tab[(*ghost).y][(*ghost).x];
                }

                tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
            }
        }

        if((*ghost).dir == 's'){
            tab[(*ghost).y][(*ghost).x] = (*ghost).under;          
            (*ghost).y += 1;
            
            if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.'){
                (*ghost).under = tab[(*ghost).y][(*ghost).x];
            }
            else{
                (*ghost).under = ' ';
            }

            tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
        }
        if((*ghost).dir == 'w'){
            tab[(*ghost).y][(*ghost).x] = (*ghost).under;          
            (*ghost).y -= 1;
            
            if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.'){
                (*ghost).under = tab[(*ghost).y][(*ghost).x];
            }
            else{
                (*ghost).under = ' ';
            }

            tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
        }
    }
}

int game_over(void){
   if(pac.x == clyde.x && pac.y == clyde.y){
    return 1;
   } 
   if(pac.x == blynk.x && pac.y == blynk.y){
    return 1;
   }
   if(pac.x == pink.x && pac.y == pink.y){
    return 1;
   }
   if(pac.x == ink.x && pac.y == ink.y){
    return 1;
   }
   return 0;
}

void spawn_ghost(Ghost (*ghost)){
    tab[(*ghost).y][(*ghost).x] = ' ';
    print_tab();
    usleep(SPEED);
    clear();

    tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
    print_tab();
    usleep(SPEED);
    clear();

    tab[(*ghost).y][(*ghost).x] = ' ';
    print_tab();
    usleep(SPEED);
    clear();

    tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
    print_tab();
    usleep(SPEED);
    clear();

    tab[(*ghost).y][(*ghost).x] = ' ';
    print_tab();
    usleep(SPEED);
    clear();

    (*ghost).x = 15;
    (*ghost).y = 11;
    (*ghost).inGame = 1;
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

    Ghost *pClyde = &clyde;
    Ghost *pBlynk = &blynk;
    Ghost *pPink  = &pink;
    Ghost *pInk   = &ink;

    while(1){

        //refresh();    NÃO USAR SE NÃO SEBE COMO
        if(gametime == 20){
            spawn_ghost(pClyde);
        }
        if(gametime == 50){
            spawn_ghost(pBlynk);
        }
        if(gametime == 80){
            spawn_ghost(pPink);
        }
        if(gametime == 110){
            spawn_ghost(pInk);
        }

        if(gametime == 130){
            clyde.dificult = 1;
        }
        if(gametime == 160){
            blynk.dificult = 1;
        }
        if(gametime == 200){
            pink.dificult = 1;
        }
        if(gametime == 230){
            ink.dificult = 1;
        }

        if(poll(&mypoll, 1, 200)){
            clear();
            keyPressed = getch();
            move_pacman();
            move_ghost(pClyde);
            move_ghost(pBlynk);
            move_ghost(pPink);
            move_ghost(pInk);
        }
        else{
            clear();
            move_pacman();
            move_ghost(pClyde);
            move_ghost(pBlynk);
            move_ghost(pPink);
            move_ghost(pInk);
        }
        print_tab();
        if(game_over())
            while(1){}
    
        clear();
        gametime++;
    }

    endwin();
}