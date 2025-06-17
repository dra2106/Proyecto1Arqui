#pragma once
#include "Entity.h"
#include "DataStructures/DLinkedList.h"
#include "Macro/macro.h"

enum Direction {
    up,
    left,
    right,
    stand
};

class Spaceship : public Entity {
private:
    Direction currentDirection;

	void loadSprite() {
        DLinkedList<string> sprite;
        sprite.append("|\\A/|");
        sprite.append("|!H!|");
        sprite.append("|/V\\|");
        setSprite(sprite);
    }

public:
    Spaceship() {
        this->x = 0;
        this->y = 0;
        currentDirection = stand;
        loadSprite();
    }

    Spaceship(int y, int x) {
        this->x = x;
        this->y = y;
        currentDirection = stand;
        loadSprite();
    }

    void setDirection(Direction d) {
        currentDirection = d;
    }

    Direction getDirection() const {
        return currentDirection;
    }

    void move() {
        switch (currentDirection) {
            case left:
                x--;
                break;
            case right:
                x++;
                break;
            default:
                break;
        }

        // Limita los bordes de la pantalla
        if (x < 1)
            x = 1;
        if (x > SCREEN_COLS - width)
            x = SCREEN_COLS - width;
    }

};
