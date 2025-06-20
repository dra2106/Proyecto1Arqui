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
    int attackCooldown = 0;
    const int BASE_COOLDOWN = 10; // 3 segundos base (60 FPS * 3)
    vector<Bullet> bullets;

    // Función auxiliar para construir el sprite
    static DLinkedList<string> buildSprite() {
        DLinkedList<string> s;
        s.append(" . .");
        s.append("/\\V/\\");
        s.append(" - -");
        return s;
    }

public:
    SmallBird(int y = 0,int x = 0)
        : Enemy(x, y, buildSprite()), health(0) {
            setRandomPattern(EnemyPatterns::SMALL_BIRD_PATTERNS); // Asigna un patrón de movimiento al azar
            attackCooldown = rand() % BASE_COOLDOWN;
        }

    // Implementación del método update
    void update() override {
        if (attackCooldown > 0) {
            attackCooldown--;
        } else {
            if (rand() % 60 == 0) {
                attack();
                attackCooldown = BASE_COOLDOWN + (rand() % 60);
            } else if (rand() % 30 == 0) {
                Enemy::update(); // Se mueve
            }
        }
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
    void attack() override {
        bullets.emplace_back(getY() + 1, getX());
        bullets.back().setDirection(DOWN);   // Las balas van hacia abajo
    }

    bool isDead() const {
        return health <= 0;
    }

    void damage() {
        health -= 1;
    }

    vector<Bullet>& getBullets() {
        return bullets;
    }

    int getHealth() const { return health; }
    void setHealth(int v) { health = v; }
};
