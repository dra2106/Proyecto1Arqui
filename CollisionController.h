#pragma once

#include "Entity.h" // Asumimos que las entidades tienen getX(), getY(), getWidth(), getHeight()

class CollisionController {
public:
    // Verifica si dos entidades están colisionando
    static bool checkCollision(const Entity& a, const Entity& b) {
        int ax1 = a.getX() - a.getWidth()  / 2;
        int ay1 = a.getY() - a.getHeight() / 2;
        int ax2 = ax1 + a.getWidth()  - 1;
        int ay2 = ay1 + a.getHeight() - 1;

        int bx1 = b.getX() - b.getWidth()  / 2;
        int by1 = b.getY() - b.getHeight() / 2;
        int bx2 = bx1 + b.getWidth()  - 1;
        int by2 = by1 + b.getHeight() - 1;

        return !(ax2 < bx1 || ax1 > bx2 || ay2 < by1 || ay1 > by2);
    }
};
