#pragma once

#include <ncurses.h>

class Screen {
private:
    WINDOW* win;

    void construct(int height, int width) {
        int xMax, yMax;
        getmaxyx(stdscr, yMax, xMax); // obtiene el máximo de la pantalla

        // el desmadre de abajo es para que el rectángulo siempre esté en el centro de la pantalla
        win = newwin(height, width, (yMax / 2) - (height / 2), (xMax / 2) - (width / 2));
    }
    
public:
    Screen() {
        construct(0, 0);
    }

    Screen(int height, int width) {
        construct(height, width);
    }

    void initialize() {
        clear();
        refresh();
    }

    void addBorder() {
        box(win, 0, 0); // el primero es izq y der, el segundo arriba y abajo
    }

    void addAt(int y, int x, char c) {
        mvwaddch(win, y, x, c);
    }

    char getInput() {
        return wgetch(win);
    }

    void clear() {
        wclear(win);
        addBorder();
    }

    void refresh() {
        wrefresh(win);
    }
    
};