#pragma once
#include <ncurses.h>
#include "Screen.h"

class MainLoop {
private:
    Screen screen;
    bool gameOver;

public:
    MainLoop(int height, int width) {
        screen = Screen(height, width);
        gameOver = false;
    }

    void processInput() {
        chtype input = screen.getInput();
        // procesar input
    }

    void updateState() {

    }

    void redraw() {
        screen.refresh();
    }

    bool isOver() {
        return gameOver;
    }

};