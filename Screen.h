#pragma once

#include <ncurses.h>
#include <string>
#include "Entity.h"
#include "DataStructures/DLinkedList.h"

class Screen {
private:
    WINDOW* win;
    int height, width;

    void construct(int height, int width) {
        
        int xMax, yMax;
        getmaxyx(stdscr, yMax, xMax); // obtiene el tamaño total del terminal

        // Si height o width son 0, usa el tamaño completo del terminal
        if (height == 0 || width == 0) {
            height = yMax;
            width = xMax;
        }

        // Calcula coordenadas para centrar la ventana
        int startY = (yMax - height) / 2;
        int startX = (xMax - width) / 2;

        // Crea la ventana centrada
        win = newwin(height, width, startY, startX);
        keypad(win, true); // habilita teclas especiales
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

public:
    Screen(int height = 0, int width = 0) {
        this->height = height;
        this->width = width;
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

    // obtiene un input del usuario
    chtype getInput() {
        return wgetch(win);
    }

    // limpia la ventana y crea la caja
    void clear() {
        wclear(win);
        addBorder();
    }

    int getWidth() const {
        return width;
    }

    // refresca la ventana
    void refresh() {
        wrefresh(win);
    }
};
