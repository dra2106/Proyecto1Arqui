#pragma once

#include "Enemy.h"
#include "Bullet.h"
#include <vector>

using std::vector;

class SmallBird : public Enemy {
private:
    int health;
    vector<Bullet> bullets;

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
        : Enemy(x, y, buildSprite()), health(1) {
            setPattern({{1,0}, {1,0}, {-1,0}, {0,1}, {0,1}, {0,1}}); // Patrón de movimiento
        }

    // Implementación del método update
    void update() override {
        Enemy::update(); // Ejecuta el comportamiento base
        
        // Comportamiento específico cuando está atacando
        if (isAttacking) {
            bullets.emplace_back(getY() + 1, getX());
            for (Bullet& b : bullets) {
                b.setDirection(DOWN); // Las balas van hacia abajo
            }
        } else {
            // Reset position if not attacking
            reset();
        }
        // reset();
    }


    // Operador de asignación
    SmallBird& operator=(const SmallBird& other) {
        if (this != &other) {
            Entity::operator=(other); // Asigna la parte base
            health = other.health;
            // Copia cualquier otro atributo propio de SmallBird aquí si es necesario
        }
        return *this;
    }

    // Implementación del método attack
    bool attack() override {
        return isAttacking = true;
        // Podrías incluir lógica de disparo o cambio de patrón
    }

    void reset() {
        isAttacking = false;
        patternIndex = 0;
        // Regresa a una posición original, por ejemplo:
        x = 0;
    }

    // string getType() const override {
    //     return "SmallBird";
    // }

    int gethealth() const { return health; }
    void sethealth(int v) { health = v; }
};
