#pragma once

#include <ncurses.h>

class Entity {
protected:
    int y, x;
    chtype icon;

public:
    Entity() {
        y = x = 0;
        icon = ' ';
    }

    Entity(int y, int x, chtype c) {
        this->y = y;
        this->x = x;
        this->icon = c;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    chtype getIcon() {
        return icon;
    }
};