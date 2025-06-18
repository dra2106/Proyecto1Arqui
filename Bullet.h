#pragma once

#include "Entity.h"
#include "Spaceship.h"
#include "Direction.h"

class Bullet : public Entity {
private:
    Direction currentDirection;

public:
    Bullet() {
        this->x = 0;
        this->y = 0;
        loadSprite();
    }

    Bullet(int y, int x) {
        this->x = x;
        this->y = y;
        loadSprite();
    }

    void move() {
        if (currentDirection == UP)
            y--;
        if (currentDirection == DOWN)
            y++;
    }

    void setDirection(Direction d) {
        currentDirection = d;
    }

private:
    void loadSprite() {
        DLinkedList<string> sprite;
        sprite.insert("|");
        setSprite(sprite);
    }
};
