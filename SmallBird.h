/* 
 * Nombre de Archivo: SmallBird.h
 *
 * Descripcion:
 * Este archivo define la clase SmallBird, que representa un enemigo pequeño en el juego.
 * Incluye la lógica para su sprite, patrones de movimiento aleatorios, disparo de balas,
 * gestión de salud y métodos de copia y asignación.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include "Enemy.h"
#include "Bullet.h"
#include "EnemyPatterns.h"
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
            setRandomPattern(EnemyPatterns::SMALL_BIRD_PATTERNS); // Asigna un patrón de movimiento al azar
        }

    // Implementación del método update
    void update() override {
        Enemy::update(); // Ejecuta el comportamiento base
        
        // Comportamiento específico cuando está atacando
        if (isAttacking) {
            bullets.emplace_back(getY() + 1, getX());
            for (Bullet& b : bullets) {
                b.setDirection(DOWN);   // Las balas van hacia abajo
                b.move();               // Mueve la bala
            }
        }
        isAttacking = false;
    }

    SmallBird(const SmallBird& other) 
        : Enemy(other),  // Llama al constructor de copia de la clase base
          health(other.health),
          bullets(other.bullets) {
    }

    // Operador de asignación
    SmallBird& operator=(const SmallBird& other) {
        if (this != &other) {
            Enemy::operator=(other); // Asigna la parte base
            health = other.health;
            bullets = other.bullets; // Copia las balas
        }
        return *this;
    }

    // Implementación del método attack
    bool attack() override {
        return isAttacking = true;
    }

    // puede eliminarse
    void reset() {
        isAttacking = false;
        patternIndex = 0;
        x = 0;
    }

    vector<Bullet>& getBullets() {
        return bullets;
    }

    int gethealth() const { return health; }
    void sethealth(int v) { health = v; }
};
