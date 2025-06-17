#pragma once

#include <ncurses.h>
#include "Entity.h"
#include "Enemy.h"

class SmallBird : public Enemy, public Entity {
public:
    SmallBird(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = 'W';
    }

    // Operador de asignación
    SmallBird& operator=(const SmallBird& other) {
        if (this != &other) {  // Evitar auto-asignación
            this->y = other.y;
            this->x = other.x;
            this->icon = other.icon;
            // Si Enemy o Entity tienen datos adicionales, podrías necesitar llamar a sus operadores también
        }
        return *this;
    }

    // Destructor si deseas liberar recursos, aunque en este caso no parece necesario
    // ~SmallBird() {
    //     icon = ' ';
    // }
};
