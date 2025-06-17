#pragma once
#include <string>

#include "DLinkedList.h"

using std::string;

class EntityX {
protected:
    int x, y; // Posición en pantalla
    int width, height; // Tamaño del objeto
    DLinkedList<string> sprite; // Representación visual (opcional)
    bool isActive; // ¿Está activa esta entidad en el juego?

public:
    EntityX(int x = 0, int y = 0, int width = 1, int height = 1)
        : x(x), y(y), width(width), height(height), isActive(true) {}

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool getIsActive() const { return isActive; }
    DLinkedList<string> getSprite() const { return sprite; }

    // Setters
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void setSize(int w, int h) { width = w; height = h; }
    void setSprite(const DLinkedList<string>& newSprite) {
        sprite = newSprite;
        height = sprite.getSize();
        sprite.goToStart();
        width = sprite.isEmpty() ? 0 : sprite.getElement().length();
    }
    void setActive(bool active) { isActive = active; }

    // Virtual destructor para herencia
    virtual ~EntityX() = default;
};
