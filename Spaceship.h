#pragma once

#include "Entity.h"
#include "DataStructures/DLinkedList.h"

class Spaceship : public Entity {
private:
    Direction currentDirection;

    void loadSprite() {
        DLinkedList<string> sprite;
        switch (currentDirection) {
            case LEFT:
                sprite.append("(\\A/|");
                sprite.append("(!H!|");
                sprite.append("(/V\\|");
                break;
            case RIGHT:
                sprite.append("|\\A/)");
                sprite.append("|!H!)");
                sprite.append("|/V\\)");
                break;
            case STAND:
            default:
                sprite.append("|\\A/|");
                sprite.append("|!H!|");
                sprite.append("|/V\\|");
                break;
        }
        setSprite(sprite);
    }

public:
    Spaceship(int y, int x) : Entity(x, y) {
        currentDirection = Direction::STAND;
        loadSprite();
    }

    void setDirection(Direction d) {
        currentDirection = d;
        loadSprite(); // Actualiza el sprite al cambiar la dirección
    }

    Direction getDirection() const { return currentDirection; }

    void move(int screenWidth, int screenHeight) {
        switch (currentDirection) {
            case LEFT:  x--; break;
            case RIGHT: x++; break;
            case UP:    y--; break;
            case DOWN:  y++; break;
            default:    break;
        }
        int offsetX = getSprite().getElement().size() / 2;
        int offsetY = getSprite().getSize() / 2;
        if (x < offsetX) x = offsetX;
        if (x > screenWidth - offsetX - 1) x = screenWidth - offsetX - 1;
        if (y < offsetY) y = offsetY;
        if (y > screenHeight - offsetY - 1) y = screenHeight - offsetY - 1;
    }
};