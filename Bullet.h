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
        currentDirection = UP;
        loadSprite();
    }

    Bullet(int y, int x) {
        this->x = x;
        this->y = y;
        currentDirection = UP;
        loadSprite();
    }

    void move() {
        if (currentDirection == UP) {
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
