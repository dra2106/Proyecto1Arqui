/* 
 * Nombre de Archivo: EnemyPatterns.h
 *
 * Descripcion:
 * Este archivo define patrones de movimiento predefinidos para los enemigos del juego,
 * almacenados como vectores de pares de enteros. Los patrones pueden ser asignados aleatoriamente
 * a los enemigos para variar su comportamiento y trayectorias en pantalla.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include <vector>

using std::pair;
using std::vector;

namespace EnemyPatterns {
    const vector<vector<pair<int, int>>> SMALL_BIRD_PATTERNS = {
        // Patrón en zigzag
        {{1,0}, {1,0}, {-1,0}, {-1,0}, {0,1}},
        
        // Patrón circular
        {{1,0}, {1,0}, {0,1}, {0,1}, {-1,0}, {-1,0}, {0,-1}, {0,-1}},
        
        // Patrón de caída
        {{0,1}, {0,1}, {1,0}, {1, 0}, {0,1}, {-1,0}, {-1,0}},
        
        // Patrón de avance rápido
        {{2,0}, {2,0}, {-1,1}, {-1,1}}
    };
}