#pragma once

#include <ncurses.h>
#include "Screen.h"
#include "Spaceship.h"
#include "CollisionController.h"

class Window {
private:
    Screen screen;
    Spaceship spaceship;
    CollisionController collision;
public:
    Window(int height, int width)
        : screen(height, width), spaceship(0, 0)
    {
        screen.initialize();
        int naveY = screen.getHeight() - 3;
        int naveX = screen.getWidth() / 2;
        spaceship = Spaceship(naveY, naveX);
        curs_set(0);
    }

    void processInput() {
        chtype input = screen.getInput();
        switch (input) {
            case KEY_LEFT:
                spaceship.setDirection(LEFT);
                break;
            case KEY_RIGHT:
                spaceship.setDirection(RIGHT);
                break;
            default:
                spaceship.setDirection(STAND);
                break;
        }
    }

    void updateState() {
        screen.clear();
        spaceship.move(screen.getWidth(), screen.getHeight());
        screen.add(spaceship);
    }

    void redraw() {
        screen.refresh();
    }
};