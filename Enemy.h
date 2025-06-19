#pragma once

#include "Entity.h"
#include <vector>
#include <time.h>

using std::vector;
using std::pair;

class Enemy : public Entity {
protected:
    int patternIndex;     // Índice del patrón actual de movimiento o ataque
    bool isAttacking;     // Indica si el enemigo está actualmente atacando
    vector<pair<int, int>> pattern;

public:
    Enemy(int x = 0, int y = 0, const DLinkedList<string>& sprite = DLinkedList<string>())
        : Entity(x, y, sprite), patternIndex(0), isAttacking(false) {
            srand(getX()); // Inicializa la semilla para números aleatorios
        }

    virtual ~Enemy() = default;

    // Constructor de copia
    Enemy(const Enemy& other) 
    : Entity(other), 
      patternIndex(other.patternIndex),
      isAttacking(other.isAttacking),
      pattern(other.pattern) {}

    // Operador de asignación
    Enemy& operator=(const Enemy& other) {
        if (this != &other) {
            Entity::operator=(other);
            patternIndex = other.patternIndex;
            isAttacking = other.isAttacking;
            pattern = other.pattern;
        }
        return *this;
    }

    void setPattern(const vector<std::pair<int, int>>& patt) {
        pattern = patt;
    }

    void setRandomPattern(const vector<vector<std::pair<int, int>>>& patterns) {
        if (!patterns.empty()) {
            int randomIndex = rand() % patterns.size();
            pattern = patterns[randomIndex];
            patternIndex = 0; // Reinicia el índice del patrón
        }

    }

    virtual void update() {
        // Movimiento basado en patrones
        if (!pattern.empty()) {
            auto [dx, dy] = pattern[patternIndex];
            x += dx;
            y += dy;

            patternIndex = (patternIndex + 1) % pattern.size();
        }
    }

    virtual bool attack() {
        return isAttacking = true;
        // Implementación específica en clases derivadas
    }

    void reset() {
        isAttacking = false;
        patternIndex = 0;
        // Regresa a una posición original, por ejemplo:
        x = 0;
    }

    int getX()& {
        return x;
    }

    int getY()& {
        return y;
    }

    // Getters y setters comunes a todos los enemigos
    int getPatternIndex() const { return patternIndex; }
    void setPatternIndex(int index) { patternIndex = index; }

    bool getIsAttacking() const { return isAttacking; }
    void setIsAttacking(bool attacking) { isAttacking = attacking; }
};
