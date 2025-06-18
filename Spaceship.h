#pragma once

#include "Entity.h"
#include "DataStructures/DLinkedList.h"

class Spaceship : public Entity {
private:
    Screen screen;
    Direction currentDirection;

	void loadSprite() {
        DLinkedList<string> sprite;
        sprite.append("|\\A/|");
        setSprite(sprite);
    }

public:
    Spaceship(Screen screen, int y = 0, int x = 0) {
        this->screen = screen;
        this->y = y;
        this->x = x;
        currentDirection = Direction::STAND;
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
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            default:
                break;
        }

        // Limita los bordes de la pantalla
        if (x < 1)
            x = 1;
        if (x > screen.getWidth() - width)
            x = screen.getWidth() - width;
    }

};
