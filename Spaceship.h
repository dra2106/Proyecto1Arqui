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
        const int margenX = 1;

        switch (currentDirection) {
            case LEFT:  x--; break;
            case RIGHT: x++; break;
            case UP:    y--; break;
            case DOWN:  y++; break;
            default:    break;
        }

        // Calcula el ancho real del sprite (la línea más larga)
        int spriteWidth = 0;
        DLinkedList<std::string> spr = getSprite();
        for (spr.goToStart(); !spr.atEnd(); spr.next()) {
            int lineWidth = spr.getElement().size();
            if (lineWidth > spriteWidth) spriteWidth = lineWidth;
        }

        if (x < margenX) x = margenX;
        if (x < margenX + spriteWidth / 2) x = margenX + spriteWidth / 2;
        if (x > screenWidth - spriteWidth - margenX)
            x = screenWidth - spriteWidth - margenX;
    }
};