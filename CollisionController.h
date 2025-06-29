/* 
 * Nombre de Archivo: CollisionController.h
 *
 * Descripcion:
 * Este archivo define la clase CollisionController, que proporciona métodos estáticos
 * para detectar colisiones entre entidades del juego, utilizando sus posiciones y dimensiones.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include "Entity.h" 
#include "Collision_arm.h"

using std::vector;
using std::pair;

class CollisionController {
public:
    bool checkCollision(const Entity& a, const Entity& b) {
        return check_collision_arm(
            a.getY(), a.getX(), a.getHeight(), a.getWidth(),  // ¡Y antes que X!
            b.getY(), b.getX(), b.getHeight(), b.getWidth()   // ¡Y antes que X!
        );
    }
};
