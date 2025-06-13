#include <iostream>
#include <string>
#include <ncurses.h>
#include "Spaceship.h"
int main() {
    initscr();
    cbreak();
    noecho();

    int height, width, start_y, start_x;
    height = 30;
    width = 50;
    start_y = 0;
    start_x = 30;

    WINDOW* win = newwin(height, width, start_y, start_x);

    box(win, 0, 0); // el primero es izq y der, el segundo arriba y abajo
    refresh();
    wrefresh(win); // refresca solo la ventana creada

    // a height se le resta 2 para que salga arriba de la caja, y width se divide
    // entre 2 para que salga en el centro
    Spaceship* p = new Spaceship(win, height-2, width/2, '@');
    do {
        p->display();
        wrefresh(win);
    } while (p->getMove() != 'x'); // escribir x para salir

    endwin();
    return 0;
}


