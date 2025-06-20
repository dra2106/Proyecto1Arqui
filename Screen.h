/* 
 * Nombre de Archivo: Screen.h
 *
 * Descripcion:
 * Este archivo define la clase Screen, encargada de administrar la ventana de juego,
 * inicializar y limpiar la pantalla, manejar la entrada del usuario y dibujar entidades
 * y sprites en pantalla utilizando la biblioteca ncurses.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include <ncurses.h>
#include <string>
#include "Entity.h"
#include "DataStructures/DLinkedList.h"

class Screen {
private:
    WINDOW* win;
    int height, width;

    // añade un caracter en las coordenadas dadas
    void addAt(int y, int x, DLinkedList<string> sprite) {
        int offsetY = sprite.getSize() / 2;
        int offsetX = sprite.getElement().size() / 2;

        // Debug: imprime coordenadas de dibujo
        //mvwprintw(win, 2, 1, "addAt: y=%d x=%d offsetY=%d offsetX=%d", y, x, offsetY, offsetX);

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
        win = newwin(height, width - 2, startY, startX);
        keypad(win, true); // habilita teclas especiales
        nodelay(win, TRUE); // <-- clave para que el juego corra solo
    }

    // inicializa la ventana
    void initialize() {
        clear();
        refresh();
        addBorder();
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

    int getHeight() const {
        return height;
    }

    void refresh() {
        wrefresh(win);
    }

    void showStringAt(const std::string& text, int row, int col) {
        if (row < 0 || row >= height)
            throw std::out_of_range("Row is out of bounds");
        if (col < 0 || col + text.size() > width)
            throw std::out_of_range("Column is out of bounds");
        if (text.empty())
            throw std::invalid_argument("Text cannot be empty");

        mvwprintw(win, row, col, "%s", text.c_str());
        wrefresh(win);
    }

    void showCenteredString(const std::string& text) {
        if (text.empty())
            throw std::invalid_argument("Text cannot be empty");

        int row = height / 2;
        int col = (width - text.size()) / 2;
        mvwprintw(win, row, col, "%s", text.c_str());
        wrefresh(win);
    }

    std::string getStringInput(const std::string& prompt) {
        char input[256]; // Buffer para la entrada
        mvwprintw(win, height - 1, 0, "%s", prompt.c_str());
        wrefresh(win);
        wgetnstr(win, input, sizeof(input) - 1); // Lee la entrada del usuario
        return std::string(input); // Devuelve la entrada como string
    }
};
