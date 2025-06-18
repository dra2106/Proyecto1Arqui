#include <iostream>
#include <string>
#include <ncurses.h>
#include "Screen.h"
#include "Spaceship.h"
#include "MainLoop.h"
#include "Entity.h"
#include "Macro/macro.h"

using std::cout;
using std::endl;

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE); // <-- clave para que el juego corra solo
    refresh();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    cols /= 3;
    MainLoop game(rows - 2, cols - 4); // centrado con márgenes
    while (!game.isOver()) {
        game.processInput();
        game.updateState();
        game.redraw();
        napms(16); // ~60 FPS
    }
    getch();
    endwin();
    cout << "Has perdido!!" << endl;
    return 0;
}
