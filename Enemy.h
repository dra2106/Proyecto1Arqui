#pragma once

#include "Entity.h"

class Enemy : public Entity {
protected:
    int patternIndex;     // Índice del patrón actual de movimiento o ataque
    bool isAttacking;     // Indica si el enemigo está actualmente atacando

    Enemy(const Enemy& other) = delete;
    void operator=(const Enemy&) = delete;

public:
    Enemy(int x = 0, int y = 0, const DLinkedList<string>& sprite = DLinkedList<string>())
        : Entity(x, y, sprite), patternIndex(0), isAttacking(false) {}

    virtual ~Enemy() = default;

    // Getters y setters comunes a todos los enemigos
    int getPatternIndex() const { return patternIndex; }
    void setPatternIndex(int index) { patternIndex = index; }

    bool getIsAttacking() const { return isAttacking; }
    void setIsAttacking(bool attacking) { isAttacking = attacking; }

    // Métodos virtuales puros
    virtual void update() = 0;
    virtual void attack() = 0;
    virtual void reset() = 0;
    virtual string getType() const = 0;
};
