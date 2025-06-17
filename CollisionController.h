#pragma once

#include "Entity.h" // Asumimos que las entidades tienen getX(), getY(), getWidth(), getHeight()

class CollisionController {
public:
    // Verifica si dos entidades están colisionando
    static bool checkCollision(Entity& a, Entity& b) {
        return !(a.getX() + a.getWidth() < b.getX() ||    // a está completamente a la izquierda de b
                 a.getX() > b.getX() + b.getWidth() ||    // a está completamente a la derecha de b
                 a.getY() + a.getHeight() < b.getY() ||   // a está completamente arriba de b
                 a.getY() > b.getY() + b.getHeight());    // a está completamente abajo de b
    }
};
