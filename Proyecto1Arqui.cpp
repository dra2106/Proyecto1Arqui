#include <iostream>
#include <string>
#include <ncurses.h>
#include "Screen.h"
#include "Spaceship.h"
#include "MainLoop.h"
#include "Entity.h"

#define SCREEN_ROWS 40
#define SCREEN_COLS 40

using std::cout;
using std::endl;

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE); // <-- clave para que el juego corra solo
    refresh();

    MainLoop game(SCREEN_ROWS, SCREEN_COLS);
    
    while (!game.isOver()) {
        game.processInput();
        game.updateState();
        game.redraw();
        napms(50); // ~20 FPS
    }

    getch();
    endwin();
    cout << "Perdio, mamon" << endl;
    return 0;
}
