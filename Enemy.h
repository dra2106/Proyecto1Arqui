// clase abstracta (idea)

#pragma once

#include "Entity.h"
#include <vector>

using std::vector;

class Enemy : public Entity {
protected:
    int patternIndex;     // Índice del patrón actual de movimiento o ataque
    bool isAttacking;     // Indica si el enemigo está actualmente atacando
    vector<std::pair<int, int>> pattern;

    Enemy(const Enemy& other) = delete;
    void operator=(const Enemy&) = delete;

public:
    Enemy(int x = 0, int y = 0, const DLinkedList<string>& sprite = DLinkedList<string>())
        : Entity(x, y, sprite), patternIndex(0), isAttacking(false) {}

    virtual ~Enemy() = default;

    void setPattern(const vector<std::pair<int, int>>& patt) {
        pattern = patt;
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

    // Getters y setters comunes a todos los enemigos
    int getPatternIndex() const { return patternIndex; }
    void setPatternIndex(int index) { patternIndex = index; }

    bool getIsAttacking() const { return isAttacking; }
    void setIsAttacking(bool attacking) { isAttacking = attacking; }
};
