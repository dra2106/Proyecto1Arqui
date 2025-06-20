/* 
 * Nombre de Archivo: Window.h
 *
 * Descripcion:
 * Este archivo define la clase Window, que administra la ventana principal del juego.
 * Se encarga de inicializar la pantalla, mostrar la nave y gestionar el control de colisiones,
 * sirviendo como interfaz principal para la visualización y control del juego.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include <string>

#include "DataStructures/DLinkedList.h"
#include "Direction.h"

using std::string;

class Entity {
protected:
    int x, y;
    int width, height; 
    DLinkedList<string> sprite; 
    bool isActive; 
    Direction direction;

public:
    Entity(int x = 0, int y = 0, const DLinkedList<string>& sprite = DLinkedList<string>())
        : x(x), y(y), isActive(true), direction(Direction::STAND) {
        setSprite(sprite);
        setPosition(x, y);
    }

    // Virtual destructor para herencia
    virtual ~Entity() = default;
        
    Entity(const Entity&) = default;  // Permite copias
    Entity& operator=(const Entity&) = default;  // Permite asignación
    
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
        int maxWidth = 0;
        for (sprite.goToStart(); !sprite.atEnd(); sprite.next()) {
            int len = sprite.getElement().length();
            if (len > maxWidth) maxWidth = len;
        }
        width = maxWidth;
    }
    void setActive(bool active) { isActive = active; }

    void move() {
        switch (direction) {
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            case STAND:
                break;
            default:
                break;
        }
    }
};
