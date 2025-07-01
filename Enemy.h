/* 
 * Nombre de Archivo: Enemy.h
 *
 * Descripcion:
 * Este archivo define la clase Enemy, que representa a los enemigos del juego.
 * Incluye la lógica para patrones de movimiento, ataques y control de estado de los enemigos.
 * Sirve como clase base para enemigos específicos con comportamientos personalizados.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include <vector>
#include <time.h>

#include "Entity.h"

using std::vector;
using std::pair;

class Enemy : public Entity {
protected:
    int patternIndex;     
    bool isAttacking;     
    vector<pair<int, int>> pattern;

public:
    Enemy(int x = 0, int y = 0, const DLinkedList<string>& sprite = DLinkedList<string>())
        : Entity(x, y, sprite), patternIndex(0), isAttacking(false) {}

    virtual ~Enemy() = default;

    Enemy(const Enemy& other) 
    : Entity(other), 
      patternIndex(other.patternIndex),
      isAttacking(other.isAttacking),
      pattern(other.pattern) {}

    Enemy& operator=(const Enemy& other) {
        if (this != &other) {
            Entity::operator=(other);
            patternIndex = other.patternIndex;
            isAttacking = other.isAttacking;
            pattern = other.pattern;
        }
        return *this;
    }

    void setPattern(const vector<pair<int, int>>& patt) {
        pattern = patt;
    }

    void setRandomPattern(const vector<vector<pair<int, int>>>& patterns) {
        if (!patterns.empty()) {
            int randomIndex = rand() % patterns.size();  
            pattern = patterns[randomIndex];
            patternIndex = 0;
        }

    }

    virtual void update() {
        if (!pattern.empty()) {
            // Compatible con C++11/14 - reemplazar structured binding
            auto movement = pattern[patternIndex];
            int dx = movement.first;
            int dy = movement.second;
            x += dx;
            y += dy;

            patternIndex = (patternIndex + 1) % pattern.size();
        }
    }

    virtual void attack() {}

    int getX()& {
        return x;
    }

    int getY()& {
        return y;
    }
    
    int getPatternIndex() const { return patternIndex; }
    void setPatternIndex(int index) { patternIndex = index; }

    bool getIsAttacking() const { return isAttacking; }
    void setIsAttacking(bool attacking) { isAttacking = attacking; }
};
