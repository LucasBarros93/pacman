#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 20
#define HEIGHT 10
#define FPS 10 // Frames por segundo

int pacmanX, pacmanY;
char grid[HEIGHT][WIDTH];

void initializeGrid() {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            grid[i][j] = '.';
        }
    }

    // Adiciona as paredes
    for (i = 0; i < HEIGHT; i++) {
        grid[i][0] = '#';
        grid[i][WIDTH - 1] = '#';
    }
    for (j = 0; j < WIDTH; j++) {
        grid[0][j] = '#';
        grid[HEIGHT - 1][j] = '#';
    }
}

void printGrid() {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            printw("%c ", grid[i][j]);
        }
        printw("\n");
    }
    refresh();
}

void movePacman(char direction) {
    int nextX = pacmanX;
    int nextY = pacmanY;

    if (direction == 'w') {
        nextY--;
    } else if (direction == 's') {
        nextY++;
    } else if (direction == 'a') {
        nextX--;
    } else if (direction == 'd') {
        nextX++;
    }

    // Verifica se a próxima posição é válida
    if (grid[nextY][nextX] != '#') {
        grid[pacmanY][pacmanX] = '.';
        pacmanX = nextX;
        pacmanY = nextY;
        grid[pacmanY][pacmanX] = 'P';
    }
}

int main() {
    char direction;

    pacmanX = 1;
    pacmanY = 1;

    initializeGrid();

    // Inicializa a biblioteca ncurses
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();

    while (1) {
        clear();
        printGrid();

        direction = getch();

        movePacman(direction);

        if (direction == 'q') {
            break;
        }

        usleep(1000000 / FPS); // Atraso para manter a taxa de atualização constante
    }

    // Finaliza a biblioteca ncurses
    endwin();

    return 0;
}