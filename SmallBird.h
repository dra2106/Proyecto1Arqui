#pragma once

#include "Enemy.h"


class SmallBird : public Enemy {
private:
    int velocity; // Velocidad de movimiento

public:
    SmallBird(int x = 0, int y = 0)
        : Enemy(x, y, 3, 3), velocity(1) // Tamaño 3x3 basado en el sprite
    {
        // Sprite del SmallBird
        DLinkedList<string> s;
        s.append(" | | ");
        s.append("  H  ");
        s.append(" | | ");
        setSprite(s);
    }

    // Implementación del método update
    void update() override {
        // Ejemplo: se mueve hacia abajo si está atacando
        if (isAttacking) {
            y += velocity;
        }
    }

    // Implementación del método attack
    void attack() override {
        isAttacking = true;
        // Podrías incluir lógica de disparo o cambio de patrón
    }

    // Reinicia el estado del enemigo
    void reset() override {
        isAttacking = false;
        patternIndex = 0;
        // Regresa a una posición original, por ejemplo:
        y = 0;
    }

    // Identificador del tipo de enemigo
    string getType() const override {
        return "SmallBird";
    }

    // Getter y Setter para la velocidad
    int getVelocity() const { return velocity; }
    void setVelocity(int v) { velocity = v; }
};
