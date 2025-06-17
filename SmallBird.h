#pragma once

#include <ncurses.h>
#include "Entity.h"
#include "Enemy.h"

class SmallBird : public Enemy, public Entity {
public:
    SmallBird(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = 'W';
    }

    // ~SmallBird() {
    //     icon = ' ';
    // }
};