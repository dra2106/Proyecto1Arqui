#pragma once

#include "Enemy.h"
#include "Bullet.h"
#include <vector>

class Mothership : public Enemy {
private:
    int health;
    

    // Función auxiliar para construir el sprite
    static DLinkedList<string> buildSprite() {
        DLinkedList<std::string> s;
        s.append("                 _______                 ");
        s.append("            T HHHH     HHHH T            ");
        s.append("           T HHHHH     HHHHH T           ");
        s.append("          T HHHHHH     HHHHHH T          ");
        return s;
    }

public:
    Mothership(int y = 0,int x = 0)
        : Enemy(x, y, buildSprite()), health(1) {
            setPattern({{0,1}, {0,0}, {0,1}, {0,0}, {0,-1}, {0,0}}); // Patrón de movimiento
        }

    void update() override {
        Enemy::update();
    }

    Mothership& operator=(const Mothership& other) {
        if (this != &other) {
            Entity::operator=(other); 
            health = other.health;
        }
        return *this;
    }

    bool attack() override {
        return isAttacking = true;
    }

    void reset() {
        isAttacking = false;
        patternIndex = 0;
        x = 0;
    }

    int gethealth() const { return health; }
    void sethealth(int v) { health = v; }
};
