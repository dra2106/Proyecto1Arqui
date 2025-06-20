/* 
 * Nombre de Archivo: Mothership.h
 *
 * Descripcion:
 * Este archivo define la clase Mothership, que representa la nave nodriza enemiga.
 * Incluye la lógica para animar el sprite del marciano dentro de la nave, gestionar su salud,
 * patrones de movimiento y detección de daño, así como la integración con el sistema de enemigos.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include <vector>

#include "Enemy.h"
#include "Bullet.h"

class Mothership : public Enemy {
private:
    int health;
    int attackCooldown = 0;
    const int BASE_COOLDOWN = 10;
    int alienIndex = 0;
    vector<Bullet> bullets;
    const std::vector<std::string> mothershipBase = {
    "                 _______                 ",
    "            T HHH{0}HHH T            ",
    "           T HHHH{1}HHHH T           ",
    "          T HHHHH{2}HHHHH T          ",
    "<HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH>",
    " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ",
    "  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  ",
    "   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ",
    "     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!     ",
    "        !!!!!!!!!!!!!!!!!!!!!!!!!        ",
    "           !!!!!!!!!!!!!!!!!!!           ",
    "               !!!!!!!!!!!               "
    };
    const std::vector<std::vector<std::string>> aliens = {
    { "  /_\\  ", " <o o> ", "  \\-/  " },
    { "  /_\\  ", " <O O> ", "  \\o/  " },
    { "  v_v  ", " <X X> ", "  \\Q/  " },
    { "  /_\\  ", " <o o> ", " .\\=/. " }
    };

    static DLinkedList<string> buildSprite() {
            DLinkedList<std::string> s;
            s.append("                 _______                 ");
            s.append("            T HHH  /_\\  HHH T            ");
            s.append("           T HHHH <o o> HHHH T           ");
            s.append("          T HHHHH  \\-/  HHHHH T          ");
            s.append("<HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH>");
            s.append(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ");
            s.append("  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  ");
            s.append("   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ");
            s.append("     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!     ");
            s.append("        !!!!!!!!!!!!!!!!!!!!!!!!!        ");
            s.append("           !!!!!!!!!!!!!!!!!!!           ");
            s.append("               !!!!!!!!!!!               ");
        return s;
    }

    DLinkedList<std::string> buildMothershipSprite(int alienIndex) {
    DLinkedList<std::string> s;
    for (size_t i = 0; i < mothershipBase.size(); ++i) {
        std::string line = mothershipBase[i];
        if (i == 1) line.replace(line.find("{0}"), 3, aliens[alienIndex][0]);
        else if (i == 2) line.replace(line.find("{1}"), 3, aliens[alienIndex][1]);
        else if (i == 3) line.replace(line.find("{2}"), 3, aliens[alienIndex][2]);
        s.append(line);
    }
    return s;
    }

public:
    Mothership(int y = 0,int x = 0)
        : Enemy(x, y, buildSprite()), health(10) {
            setPattern({{0,1}, {0,0}, {0,1}, {0,0}, {0,-1}, {0,0}});
        }

    void update() override {
        alienIndex = (alienIndex + 1) % aliens.size();
        if (attackCooldown > 0) {
            attackCooldown--;
        } else {
            if (rand() % 20 == 0) {
                attack();
                attackCooldown = BASE_COOLDOWN + (rand() % 60);
            } else if (rand() % 30 == 0) {
                Enemy::update(); // Se mueve
                setSprite(buildMothershipSprite(alienIndex));
            }
        }
    }

    Mothership(const Mothership& other) 
    : Enemy(other),
      health(other.health),
      attackCooldown(other.attackCooldown),
      alienIndex(other.alienIndex),
      bullets(other.bullets) {
    }

    Mothership& operator=(const Mothership& other) {
        if (this != &other) {
            Entity::operator=(other); 
            health = other.health;
        }
        return *this;
    }

    void attack() override {
        bullets.emplace_back(getY() + 5, getX());
        bullets.back().setDirection(DOWN);   // Las balas van hacia abajo
    }

    vector<Bullet>& getBullets() {
        return bullets;
    }

    int getHealth() const { return health; }
    
    void damage() { this->health -= 1; }

    bool isDead() const { return health <= 0; }

    void sethealth(int v) { health = v; }
};