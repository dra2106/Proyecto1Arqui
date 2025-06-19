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
    int alienIndex = 0;
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
        setSprite(buildMothershipSprite(alienIndex));
        Enemy::update();
    }

    Mothership& operator=(const Mothership& other) {
        if (this != &other) {
            Entity::operator=(other); 
            health = other.health;
        }
        return *this;
    }

    bool attack() override {
        return isAttacking = true;
    }

    int gethealth() const { return health; }
    
    void damage() { this->health -= 1; }

    bool isDead() const { return health <= 0; }

    void sethealth(int v) { health = v; }
};