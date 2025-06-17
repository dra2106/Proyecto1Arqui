#include <iostream>
#include <string>
#include <ncurses.h>
#include "Screen.h"
#include "Spaceship.h"
#include "MainLoop.h"
#include "Entity.h"

#define SCREEN_ROWS 28
#define SCREEN_COLS 30

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    refresh();

    MainLoop game(SCREEN_ROWS, SCREEN_COLS);
    
    while (!game.isOver()) {
        game.processInput();

        game.updateState();

        game.redraw();
    }
    
    getch();
    endwin();
    return 0;
}
