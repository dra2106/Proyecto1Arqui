#pragma once

#include <ncurses.h>
#include <string>
#include "Entity.h"
#include "DataStructures/DLinkedList.h"

class Screen {
private:
    WINDOW* win;

    void construct(int height, int width) {
        int xMax, yMax;
        getmaxyx(stdscr, yMax, xMax); // obtiene el máximo de la pantalla

        // el desmadre de abajo es para que el rectángulo siempre esté en el centro de la pantalla
        win = newwin(height, width, (yMax / 2) - (height / 2), (xMax / 2) - (width / 2));
        keypad(win, true); // permite usar las teclas
    }
    
public:
    Screen() {
        construct(0, 0);
    }

    Screen(int height, int width) {
        construct(height, width);
    }

    // inicializa la ventana
    void initialize() {
        clear();
        refresh();
    }

    // crea la caja donde estará contenido el juego
    void addBorder() {
        box(win, 0, 0); // el primero es izq y der, el segundo arriba y abajo
    }

    // añade una entidad en las coordenadas dadas
    void add(Entity entity) {
        addAt(entity.getY(), entity.getX(), entity.getSprite());
    }

    // añade un caracter en las coordenadas dadas
    void addAt(int y, int x, DLinkedList<string> sprite) {
        int offsetY = sprite.getSize() / 2;
        int offsetX = sprite.getElement().size() / 2;

        int i = 0;
        for (sprite.goToStart(); !sprite.atEnd(); sprite.next()) {
            string line = sprite.getElement();
            for (int j = 0; j < line.size(); ++j) {
                char c = line[j];
                mvwaddch(win, y + i - offsetY, x + j - offsetX, c);
            }
            i++;
        }
    }

    // obtiene un input del usuario
    chtype getInput() {
        return wgetch(win);
    }

    // limpia la ventana y crea la caja
    void clear() {
        wclear(win);
        addBorder();
    }

    // refresca la ventana
    void refresh() {
        wrefresh(win);
    }
};
