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
    refresh();

    MainLoop game(SCREEN_ROWS, SCREEN_COLS);
    
    while (!game.isOver()) {
        // game.processInput();

        game.updateState();

        game.redraw();
    }

    // a height se le resta 2 para que salga arriba de la caja, y width se divide
    // entre 2 para que salga en el centro
    // Spaceship* p = new Spaceship(screen, -2, width/2, '@');
    // do {
    //     p->display();
    //     wrefresh(win);
    // } while (p->getMove() != 'x'); // escribir x para salir
    getch();
    endwin();
    return 0;
}
