#pragma once
#include <ncurses.h>
#include "Screen.h"
#include "Entity.h"
#include "SmallBird.h"
#include "Spaceship.h"

class MainLoop {
private:
    Screen screen;
    bool gameOver;
    Spaceship spaceship;

public:
    MainLoop(int height, int width) {
        screen = Screen(height, width);
        screen.initialize();
        gameOver = false;
        spaceship = Spaceship(26, 15);
    }

    // procesa el input del usuario
    void processInput() {
        chtype input = screen.getInput();
        switch (input) {
            case KEY_UP: // por ahora no hace nada, IMPLEMENTAR ESCUDO
                break;
            case KEY_DOWN:
                break;  // no hace nada
            case KEY_LEFT:
                spaceship.setDirection(left);
                break;
            case KEY_RIGHT:
                spaceship.setDirection(right);
                break;
        }
    }

    // actualiza el estado de la pantalla
    void updateState() {
        screen.clear(); // limpia la pantalla para evitar residuos

        spaceship.move(); // mueve la nave
        screen.add(SmallBird(5, 15));
        screen.add(spaceship); // muestra la nave en pantalla
    }

    // refresca la pantalla
    void redraw() {
        screen.refresh();
    }

    // dicta si el juego ya terminó
    bool isOver() {
        return gameOver;
    }
};
