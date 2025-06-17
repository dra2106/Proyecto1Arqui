#pragma once

#include "Entity.h"
#include "Spaceship.h"

class Bullet : public Entity {
private:
    Direction currentDirection;

public:
    Bullet() {
        this->x = 0;
        this->y = 0;
        currentDirection = up;
        loadSprite();
    }

    Bullet(int y, int x) {
        this->x = x;
        this->y = y;
        currentDirection = up;
        loadSprite();
    }

    void move() {
        if (currentDirection == up) {
            y--;
        }
        // Podés extender con más direcciones si después hacés balas enemigas.
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
