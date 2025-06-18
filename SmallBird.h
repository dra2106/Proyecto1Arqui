#pragma once

#include "Enemy.h"

class SmallBird : public Enemy {
private:
    int velocity; // Velocidad de movimiento

    // Función auxiliar para construir el sprite
    static DLinkedList<string> buildSprite() {
        DLinkedList<string> s;
        s.append(" | | ");
        s.append("  H  ");
        s.append(" | | ");
        return s;
    }

public:
    SmallBird(int y = 0,int x = 0)
        : Enemy(x, y, buildSprite()), velocity(1) {}

    // Implementación del método update
    void update() override {
        if (isAttacking) {
            y += velocity;
        }
    }


    // Operador de asignación
    SmallBird& operator=(const SmallBird& other) {
        if (this != &other) {
            Entity::operator=(other); // Asigna la parte base
            velocity = other.velocity;
            // Copia cualquier otro atributo propio de SmallBird aquí si es necesario
        }
        return *this;
    }

    void attack() override {
        isAttacking = true;
    }

    void reset() override {
        isAttacking = false;
        patternIndex = 0;
        y = 0;
    }

    string getType() const override {
        return "SmallBird";
    }

    int getVelocity() const { return velocity; }
    void setVelocity(int v) { velocity = v; }
};
