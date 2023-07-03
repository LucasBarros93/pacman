#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <poll.h>
#include <time.h>

#define SPEED  500000
#define RESETCOLOR "\x1B[0m"

#define ROWS  33
#define COLS  28

char tab[ROWS][COLS];
char keyPressed = ' ';
int  gametime = 1;
int  points = 0;
int  lvl = 1;

//CRIANDO E INICIANDO O PACMAN COMO UM STRUCT
typedef struct{
    int x;
    int y;
    char dir;
} Pacman;

Pacman pac = {14, 24, 'a'};

//CRIANDO E INICIANDO FANTASMAS
typedef struct{
    char symbol;
    int x;
    int y;
    char dir;
    char under;
    int inGame;
    int dificult;
    int powerless;
} Ghost;

Ghost clyde = {'C', 12, 15, 'a', ' ', 0, 0, 0};
Ghost blynk = {'B', 13, 15, 'a', ' ', 0, 0, 0};
Ghost pink  = {'P', 14, 15, 'a', ' ', 0, 0, 0};
Ghost ink   = {'I', 15, 15, 'a', ' ', 0, 0, 0};

struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };

//LÊ O ARQUIVO DO MAPA E O TRANSFORMA NA VARIAVEL tab[ROWS][COLS]
void init_tab(void){
    FILE *fp;
    long lSize;

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

//PRINTA O MAPA NA TELA, SUBSTITUINDO OS CAARACTERES PADRÕES PELOS ESPECIAIS E DANDO AS DEVIDAS CORES
void print_tab(void){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            if(i == 0 || i == ROWS-1){
                if(tab[i][j] == 'O'){
                    printf("\x1b[38;5;11m" "•");
                    continue;
                }
                printf("\x1b[38;5;11m" "%c", tab[i][j]);
                continue;
            }

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
                if(clyde.powerless){
                    printf("\x1b[38;5;26m" "ᗝ");
                    continue;
                }
                printf("\x1b[38;5;214m" "ᗝ");
                continue;
            }
            if(tab[i][j] == 'B'){
                if(blynk.powerless){
                    printf("\x1b[38;5;26m" "ᗝ");
                    continue;
                }
                printf("\x1b[38;5;197m" "ᗝ");
                continue;
            }
            if(tab[i][j] == 'P'){
                if(pink.powerless){
                    printf("\x1b[38;5;26m" "ᗝ");
                    continue;
                }
                printf("\x1b[38;5;213m" "ᗝ");
                continue;
            }
            if(tab[i][j] == 'I'){
                if(ink.powerless){
                    printf("\x1b[38;5;26m" "ᗝ");
                    continue;
                }
                printf("\x1b[38;5;81m" "ᗝ");
                continue;
            }

            printf("%c", tab[i][j]);
        }

        printf("\n\r");
    }
}

//MOVE O PACMAN, EVITA A COLIÇÃO COM AS PAREDES E REALIZA O TELEPORTE SE NECESSARIO
char move_pacman(void){
    char food = ' ';
    if(keyPressed == 'd'){
        if(pac.x+1 > COLS-1){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';
            pac.x = 0;
            
            food = tab[pac.y][pac.x];
            tab[pac.y][pac.x] = 'A';
        }

        else if(tab[pac.y][pac.x+1] != '#'){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';
            pac.x += 1;
            
            food = tab[pac.y][pac.x];
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
            
            food = tab[pac.y][pac.x];
            tab[pac.y][pac.x] = 'A';
        }

        else if(tab[pac.y][pac.x-1] != '#'){
            pac.dir = keyPressed;

            tab[pac.y][pac.x] = ' ';
            pac.x -= 1;
            
            food = tab[pac.y][pac.x];
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
            
            food = tab[pac.y][pac.x];
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
            
            food = tab[pac.y][pac.x];
            tab[pac.y][pac.x] = 'A';
        }
        else{
            keyPressed = pac.dir;
        }
    }
    return food;
}

//MOVE O FANTASMA, EVITA COLISÃO COM AS PAREDES E ESCOLHE A DIREÇÃO DE FORMA ALEATÓRIA OU NÃO
void move_ghost(Ghost (*ghost)){
    if((*ghost).inGame){
        int options = 1;
        char directions[4];
        directions[0] = (*ghost).dir;
        srand(time(NULL));

        if((*ghost).dir == 'd'|| (*ghost).dir == 'a'){
            if(tab[(*ghost).y+1][(*ghost).x] != '#' || tab[(*ghost).y-1][(*ghost).x] != '#'){
                options = 0;
                if (tab[(*ghost).y+1][(*ghost).x] == ' ' || tab[(*ghost).y+1][(*ghost).x] == 'A' || tab[(*ghost).y+1][(*ghost).x] == '.' || tab[(*ghost).y+1][(*ghost).x] == 'o'){
                    directions[options] = 's';
                    options++;
                }
                if (tab[(*ghost).y-1][(*ghost).x] == ' ' || tab[(*ghost).y-1][(*ghost).x] == 'A' || tab[(*ghost).y-1][(*ghost).x] == '.' || tab[(*ghost).y-1][(*ghost).x] == 'o'){
                    directions[options] = 'w';
                    options++;
                }
                if (tab[(*ghost).y][(*ghost).x+1] == ' ' || tab[(*ghost).y][(*ghost).x+1] == 'A' || tab[(*ghost).y][(*ghost).x+1] == '.' || tab[(*ghost).y][(*ghost).x+1] == 'o'){
                    directions[options] = 'd';
                    options++;
                }
                if (tab[(*ghost).y][(*ghost).x-1] == ' ' || tab[(*ghost).y][(*ghost).x-1] == 'A' || tab[(*ghost).y][(*ghost).x-1] == '.' || tab[(*ghost).y][(*ghost).x-1] == 'o'){
                    directions[options] = 'a';
                    options++;
                }
            }
        }

        else{
            if(tab[(*ghost).y][(*ghost).x+1] != '#' || tab[(*ghost).y][(*ghost).x-1] != '#'){
                options = 0;
                if (tab[(*ghost).y+1][(*ghost).x] == ' ' || tab[(*ghost).y+1][(*ghost).x] == 'A' || tab[(*ghost).y+1][(*ghost).x] == '.' || tab[(*ghost).y+1][(*ghost).x] == 'o'){
                    directions[options] = 's';
                    options++;
                }
                if (tab[(*ghost).y-1][(*ghost).x] == ' ' || tab[(*ghost).y-1][(*ghost).x] == 'A' || tab[(*ghost).y-1][(*ghost).x] == '.' || tab[(*ghost).y-1][(*ghost).x] == 'o'){
                    directions[options] = 'w';
                    options++;
                }
                if (tab[(*ghost).y][(*ghost).x+1] == ' ' || tab[(*ghost).y][(*ghost).x+1] == 'A' || tab[(*ghost).y][(*ghost).x+1] == '.' || tab[(*ghost).y][(*ghost).x+1] == 'o'){
                    directions[options] = 'd';
                    options++;
                }
                if (tab[(*ghost).y][(*ghost).x-1] == ' ' || tab[(*ghost).y][(*ghost).x-1] == 'A' || tab[(*ghost).y][(*ghost).x-1] == '.' || tab[(*ghost).y][(*ghost).x-1] == 'o'){
                    directions[options] = 'a';
                    options++;
                }
            }
        }
        
        //ESCOLHENDO A DIREÇÃO DE FORMA INTELIGENTE
        int finalOptions = 0;
        char finalDirections[4];

        if((*ghost).dificult && (*ghost).powerless == 0){
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

        //POSIÇÃO ALEATORIA ENTRE AS OPÇÕES SELECIONADAS ATÉ AQUI
        if(finalOptions == 0){
            (*ghost).dir = directions[rand()%(options)];
        }
        else{
            (*ghost).dir = finalDirections[rand()%(finalOptions)];
        }

        //MOVE O FANTASMA DE FATO
        if((*ghost).dir == 'd'){
            if((*ghost).x+1 > COLS-1){
                tab[(*ghost).y][(*ghost).x] = (*ghost).under;
                (*ghost).x = 0;
                
                if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.' || tab[(*ghost).y][(*ghost).x] == 'o'){
                    (*ghost).under = tab[(*ghost).y][(*ghost).x];
                }

                tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
            }

            else{
                tab[(*ghost).y][(*ghost).x] = (*ghost).under;
                (*ghost).x += 1;
                
                if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.' || tab[(*ghost).y][(*ghost).x] == 'o'){
                    (*ghost).under = tab[(*ghost).y][(*ghost).x];
                }

                tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
            }
        }
        if((*ghost).dir == 'a'){
            if((*ghost).x-1 < 0){
                tab[(*ghost).y][(*ghost).x] = (*ghost).under;
                (*ghost).x = COLS-1;
                
                if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.' || tab[(*ghost).y][(*ghost).x] == 'o'){
                    (*ghost).under = tab[(*ghost).y][(*ghost).x];
                }

                tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
            }

            else{
                tab[(*ghost).y][(*ghost).x] = (*ghost).under;
                (*ghost).x -= 1;
                
                if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.' || tab[(*ghost).y][(*ghost).x] == 'o'){
                    (*ghost).under = tab[(*ghost).y][(*ghost).x];
                }

                tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
            }
        }

        if((*ghost).dir == 's'){
            tab[(*ghost).y][(*ghost).x] = (*ghost).under;          
            (*ghost).y += 1;
            
            if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.' || tab[(*ghost).y][(*ghost).x] == 'o'){
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
            
            if(tab[(*ghost).y][(*ghost).x] == ' ' || tab[(*ghost).y][(*ghost).x] == '.' || tab[(*ghost).y][(*ghost).x] == 'o'){
                (*ghost).under = tab[(*ghost).y][(*ghost).x];
            }
            else{
                (*ghost).under = ' ';
            }

            tab[(*ghost).y][(*ghost).x] = (*ghost).symbol;
        }
    }
}

//TESTA A COLISÃO ENTRE O FANTASMA E O PACMAN
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

//SPAWAN O FANTASMA
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
    (*ghost).y = 12;
    (*ghost).inGame = 1;
}

//RESETA A TELA, REINICIANDO TODAS AS POCISÕES E BOLINHAS COMIDAS
void hard_reset(void){
    clear();
    refresh();
    usleep(SPEED/2);
    print_tab();
    usleep(SPEED);

    clear();
    refresh();
    usleep(SPEED/2);
    print_tab();
    usleep(SPEED);

    clear();
    refresh();
    usleep(SPEED/2);
    print_tab();
    usleep(SPEED);
    clear();
    refresh();
    
    init_tab();
    pac.x = 14;
    pac.y = 24;
    pac.dir = 'a';

    clyde.x = 12;
    clyde.y = 15;    
    clyde.dir = 'a';
    clyde.inGame = 0;
    clyde.dificult = 0;

    blynk.x = 13;
    blynk.y = 15;    
    blynk.dir = 'a';
    blynk.inGame = 0;
    blynk.dificult = 0;

    pink.x = 14;
    pink.y = 15;    
    pink.dir = 'a';
    pink.inGame = 0;
    pink.dificult = 0;

    ink.x = 15;
    ink.y = 15;    
    ink.dir = 'a';
    ink.inGame = 0;
    ink.dificult = 0;

    gametime = 1;
    lvl += 1;
    points += 500;

    print_tab();
    usleep(SPEED*3); 
}

//COLOCA A PONTUAÇÃO E O LEVEL ATUAL NO TABULEIRO
void lvl_points(void){
    int aux1;
    
    aux1 = points/10000;
    tab[32][7] = aux1+'0';

    aux1 = (points/1000)%10;
    tab[32][8] = aux1+'0';

    aux1 = ((points/100)%100)%10;
    tab[32][9] = aux1+'0';
    
    aux1 = (((points/10)%1000)%100)%10;
    tab[32][10] = aux1+'0';

    aux1 = (((points%10000)%1000)%100)%10;
    tab[32][11] = aux1+'0';

    aux1 = lvl/10;
    tab[32][26] = aux1+'0';

    aux1 = lvl%10;
    tab[32][27] = aux1+'0';
}

//SOMA OS PONTOS ADQUIRIDOS E GERENCIA O "SUPER PACMAN"
void pontuation_power(char food){
    if(food == '.'){
        points += 10;
    }

    //"SUPER PACMAN", BASICAMENTE É IGUAL A MAIN, CONTUDO SEM O game_over() E DURANDO 50 MOVIMENTOS
    if(food == 'o'){
        Ghost *pClyde = &clyde;
        Ghost *pBlynk = &blynk;
        Ghost *pPink  = &pink;
        Ghost *pInk   = &ink;
        char foo = ' ';

        clyde.powerless = 1;
        blynk.powerless = 1;
        pink.powerless = 1;
        ink.powerless = 1;

        int time = 0;
        while(time < 50){
            if(poll(&mypoll, 1, 200)){
                clear();
                keyPressed = getch();
                move_ghost(pClyde);
                move_ghost(pBlynk);
                move_ghost(pPink);
                move_ghost(pInk);
                foo = move_pacman();
            }
            else{
                clear();
                move_ghost(pClyde);
                move_ghost(pBlynk);
                move_ghost(pPink);
                move_ghost(pInk);
                foo = move_pacman();
            }
            pontuation_power(foo);
            lvl_points();
            print_tab();
            time++;
        }

        clyde.powerless = 0;
        blynk.powerless = 0;
        pink.powerless = 0;
        ink.powerless = 0;
    }

    if(food == 'C'){
        clyde.inGame = 0;
        clyde.x = 12;
        clyde.y = 15;
        clyde.dir = 'a';
        clyde.under = ' ';

        tab[15][12] = 'C';

        points += 100;
    }
    if(food == 'B'){
        blynk.inGame = 0;
        blynk.x = 13;
        blynk.y = 15;
        blynk.dir = 'a';
        blynk.under = ' ';
        
        tab[15][13] = 'B';

        points += 100;
    }
    if(food == 'P'){
        pink.inGame = 0;
        pink.x = 14;
        pink.y = 15;
        pink.dir = 'a';
        pink.under = ' ';

        tab[15][14] = 'P';

        points += 100;
    }
    if(food == 'I'){
        ink.inGame = 0;
        ink.x = 15;
        ink.y = 15;
        ink.dir = 'a';
        ink.under = ' ';

        tab[15][15] = 'I';

        points += 100;
    }

}

//TESTA SE ACABARAM AS BOLINHAS DO MAPA
int isOver(void){
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLS; j++){
            if(tab[i][j] == '.'){
                return 0;
            }
        }
    }
    return 1;
}

//SALVA O MAPA FINAL EM UM OUTRO ARQUIVO
void save_res(void){
    FILE *fp = fopen("res.txt", "w");
    if (fp == NULL)
        printf("Error opening the file");

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(fp, "%c", tab[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

//EXECUTA O PROGRAMA
void main(void){
    curs_set (0);  // Hide Cursor 

    initscr();

    keypad(stdscr, TRUE);

    noecho();

    clear();
    refresh();

    init_tab();
    print_tab();

    Ghost *pClyde = &clyde;
    Ghost *pBlynk = &blynk;
    Ghost *pPink  = &pink;
    Ghost *pInk   = &ink;
    int gameover = 0;
    char food = ' ';

    //LOOP PRINCIPAL
    while(1){
        
        //TEMPOS DE SPAWN E INTELIGENCIA DOS FANTASMAS
        if(gametime%20 == 0 && !clyde.inGame){
            spawn_ghost(pClyde);
        }
        if(gametime%50 == 0 && !blynk.inGame){
            spawn_ghost(pBlynk);
        }
        if(gametime%80 == 0 && !pink.inGame){
            spawn_ghost(pPink);
        }
        if(gametime%110 == 0 && !ink.inGame){
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
            food = move_pacman();
            gameover = game_over();
            move_ghost(pClyde);
            move_ghost(pBlynk);
            move_ghost(pPink);
            move_ghost(pInk);
        }
        else{
            clear();
            food = move_pacman();
            gameover = game_over();
            move_ghost(pClyde);
            move_ghost(pBlynk);
            move_ghost(pPink);
            move_ghost(pInk);
        }
        print_tab();

        //TESTE DE PÓS MOVIMENTO
        if(gameover){
            save_res();
            while(1){}
        }
        if(isOver()){
            hard_reset();
        }
        pontuation_power(food);
        lvl_points();
    
        clear();
        gametime++;
    }

    endwin();
}