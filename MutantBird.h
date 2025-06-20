/* 
 * Nombre de Archivo: MutantBird.h
 *
 * Descripcion:
 * Este archivo define la clase MutantBird, que representa un enemigo especial con animaciones
 * de sprite y patrones de movimiento únicos. Incluye la lógica para cambiar de sprite, disparar balas,
 * gestionar su salud y controlar su comportamiento durante el juego.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include <vector>
#include "DataStructures/DLinkedList.h"
#include "EnemyPatterns.h"
#include "Enemy.h"
#include "Bullet.h"

class MutantBird : public Enemy {
private:
    int health;
    int animationIndex = 0;
    int counter = 0;
    int attackCooldown = 0;
    const int BASE_COOLDOWN = 10; // 3 segundos base (60 FPS * 3)
    int animationDelay = 60; // Cambia de sprite cada 10 frames
    int animationCounter = 0;
    std::vector<DLinkedList<std::string>> animationSprites;
    std::vector<Bullet> bullets;

    void loadSprites() {
        // Sprite 1
        DLinkedList<std::string> s1;
        s1.append("  o  ");
        animationSprites.push_back(s1);

        // Sprite 2
        DLinkedList<std::string> s2;
        s2.append(" / \\ ");
        s2.append(" \\ / ");
        animationSprites.push_back(s2);

        // Sprite 3
        DLinkedList<std::string> s3;
        s3.append("  /\\ /\\  ");
        s3.append(" /  A  \\ ");
        s3.append(" \\/ V \\/ ");
        s3.append("  Y Y   ");
        animationSprites.push_back(s3);

        // Sprite 4
        DLinkedList<std::string> s4;
        s4.append(" /\\   /\\ ");
        s4.append("/   A   \\");
        s4.append("\\/  V  \\/");
        s4.append("  Y   Y  ");
        animationSprites.push_back(s4);

        // Sprite 5
        DLinkedList<std::string> s5;
        s5.append(" /\\   /\\ ");
        s5.append("/  \\A/  \\");
        s5.append("\\/\\ V /\\/");
        s5.append("   Y Y   ");
        animationSprites.push_back(s5);
    }

    void updateSprite() {
        setSprite(animationSprites[animationIndex]);
    }

public:
    MutantBird(int y = 0, int x = 0)
        : Enemy(x, y), health(2)
    {
        loadSprites();
        updateSprite();
        setRandomPattern(EnemyPatterns::MUTANT_BIRD_PATTERNS);
        attackCooldown = rand() % BASE_COOLDOWN;
    }

    void update() override {
        animationCounter++;
        if (animationCounter >= animationDelay) {
            animationCounter = 0;
            animationIndex = (animationIndex + 1) % animationSprites.size();
            updateSprite();
        }
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

    void setSprite(const DLinkedList<string>& newSprite) {
        Entity::setSprite(newSprite);
        if (height == 1 || height == 1) {
            health = 0; // Si el sprite es pequeño, la salud es 0 (que en realidad es 1)
        }
        else {
            health = 2; // Si el sprite es mediano, la salud es 2
        }
    }

    MutantBird(const MutantBird& other) 
        : Enemy(other),
          health(other.health),
          animationIndex(other.animationIndex),
          counter(other.counter),
          animationSprites(other.animationSprites), // Copia los sprites
          bullets(other.bullets)
    {
        updateSprite(); // Asegura que el sprite actual sea correcto
    }

    MutantBird& operator=(const MutantBird& other) {
        if (this != &other) {
            Enemy::operator=(other);
            health = other.health;
            animationIndex = other.animationIndex;
            counter = other.counter;
            animationSprites = other.animationSprites;
            bullets = other.bullets;
            updateSprite();
        }
        return *this;
    }

    void attack() override {
        bullets.emplace_back(getY() + 1, getX());
        bullets.back().setDirection(DOWN); // Las balas van hacia abajo
        
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
