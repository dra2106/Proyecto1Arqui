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
            srand(getX()); // Inicializa la semilla para números aleatorios, es en getX() porque así me dijo mi compa y funciona XD
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

    void setPattern(const vector<pair<int, int>>& patt) {
        pattern = patt;
    }

    // asigna un patrón dentro del vector de patrones
    void setRandomPattern(const vector<vector<pair<int, int>>>& patterns) {
        if (!patterns.empty()) {
            int randomIndex = rand() % patterns.size();     // posición al azar dentro de patterns
            pattern = patterns[randomIndex];
            patternIndex = 0;                               // Reinicia el índice del patrón
        }

    }

    // actualiza los patrones
    virtual void update() {
        if (!pattern.empty()) {
            auto [dx, dy] = pattern[patternIndex];
            x += dx;
            y += dy;

            patternIndex = (patternIndex + 1) % pattern.size();
        }
    }

    // asigna que el enemigo está atacando
    virtual bool attack() {
        return isAttacking = true;
    }

    // resetea el estado (no se usa por ahora)
    void reset() {
        isAttacking = false;
        patternIndex = 0;
        x = 0;
    }

    // Getters y setters comunes a todos los enemigos
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
