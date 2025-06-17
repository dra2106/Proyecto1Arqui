#pragma once
#include <string>

#include "DataStructures/DLinkedList.h"

using std::string;

class Entity {
protected:
    int x, y;
    int width, height; 
    DLinkedList<string> sprite; 
    bool isActive; 

public:
    Entity(int x = 0, int y = 0, int width = 1, int height = 1)
        : x(x), y(y), width(width), height(height), isActive(true) {}

    // Virtual destructor para herencia
    virtual ~Entity() = default;
        
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
};
