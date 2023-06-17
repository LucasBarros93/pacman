#include <ncurses.h>

int main()
{
    char tecla;

    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    while (true) 
    {
        tecla = getch();
        if (tecla >= ' ') 
            printw("You pressed %c\n\r",tecla);
        if (tecla == 'x') 
            break;
        napms(100);
        printw("Running           \r");
    }
    endwin();
    return 0; 
}

