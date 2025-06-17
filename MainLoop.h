#pragma once
#include <ncurses.h>
#include "Screen.h"
#include "Entity.h"
#include "SmallBird.h"

class MainLoop {
private:
    Screen screen;
    bool gameOver;

public:
    MainLoop(int height, int width) {
        screen = Screen(height, width);
        screen.initialize();
        gameOver = false;
    }

    void processInput() {
        chtype input = screen.getInput();
        // procesar input
    }

    // actualiza el estado de la pantalla
    void updateState() {
        screen.add(SmallBird(5, 15));
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