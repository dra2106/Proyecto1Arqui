/* 
 * Nombre de Archivo: Screen.h
 *
 * Descripcion:
 * Este archivo define la clase Screen, encargada de administrar la ventana de juego,
 * inicializar y limpiar la pantalla, manejar la entrada del usuario y dibujar entidades
 * y sprites en pantalla utilizando la biblioteca ncurses.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include <ncurses.h>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <time.h>
#include "Screen.h"
#include "Entity.h"
#include "SmallBird.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "EnemyController.h"
#include "CollisionController.h"

using std::runtime_error;
using std::vector;

class MainLoop {
private:
    Screen screen;
    bool gameOver;
    Spaceship spaceship;
    CollisionController collision;
    EnemyController enemyCon;
    vector<Bullet> bullets;
    vector<SmallBird> smallBirds;
    int vidas = 9;

public:
    MainLoop(int height, int width)
        : screen(height, width),
        gameOver(false),
        spaceship(0, 0) // valores temporales
    {
        screen.initialize();
        int naveY = screen.getHeight() - 3;     // 2 bloques arriba del borde inferior
        int naveX = screen.getWidth() / 2;      // centrada horizontalmente
        spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
        curs_set(0);                            // Oculta cursor
    }
    void ConstruirNiveles(int nivel) {
        if (nivel == 1) {
            screen.initialize();
            enemyCon.spawnSmall(screen.getHeight(), screen.getWidth(), 3, screen.getWidth() / 2);

            int naveY = screen.getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = screen.getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0); 
        } else if (nivel == 2) {
            screen.initialize();
            enemyCon.spawnSmall(screen.getHeight(), screen.getWidth(), nivel, screen.getWidth() / 2);
            int naveY = screen.getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = screen.getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
        } else if (nivel == 3) {
            screen.initialize();
            enemyCon.spawnMutant(screen.getHeight(), screen.getWidth(), nivel, screen.getWidth() / 2);
            
            int naveY = screen.getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = screen.getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
            
        } else if (nivel == 4) {
            screen.initialize();
            enemyCon.spawnMutant(screen.getHeight(), screen.getWidth(), nivel, screen.getWidth() / 2);
            
            int naveY = screen.getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = screen.getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
        } else if (nivel == 5) {
            
        }


    }
    // procesa el input del usuario
    void processInput() {
        chtype input = screen.getInput();
        switch (input) {
            case KEY_UP:                // por ahora no hace nada, IMPLEMENTAR ESCUDO
                spaceship.setDirection(STAND);    
                break;
            case KEY_DOWN:
                spaceship.setDirection(STAND);
                break;                  // no hace nada
            case KEY_LEFT:
                spaceship.setDirection(LEFT);
                break;
            case KEY_RIGHT:
                spaceship.setDirection(RIGHT);
                break;
            case ' ': // espacio es la tecla para disparar
                spaceship.setDirection(STAND);    
                if (bullets.size() < 5) {       // límite de 5 balas en pantalla
                    bullets.emplace_back(spaceship.getY() - 1, spaceship.getX());
                    for (Bullet& b : bullets) {
                        b.setDirection(UP);     // todas las balas van hacia arriba
                    }
                }
                break;
            default:
                spaceship.setDirection(STAND);
                break;
        }
    }

    void updateState() {
        screen.clear();         // limpia la pantalla para evitar residuos
        spaceship.move(screen.getWidth(), screen.getHeight()); // mueve la nave usando el ancho de la pantalla
        // si una nave choca con un enemigo, pierde

        if (enemyCon.checkPlayerCollisions(spaceship)){
            vidas--; // si hay colisión, se resta una vida
            if (vidas < 1) {
                gameOver = true;
            }
        }

        enemyCon.CheckCollisionsEnemies(bullets);
        enemyCon.updateAll(screen); // actualiza todos los enemigos
        
        // Mueve y dibuja las balas activas
        for (Bullet& b : bullets) {
            b.move();
            screen.add(b);
        }
        // Elimina balas que se salen de la pantalla por arriba
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return b.getY() < 1; }),
            bullets.end()
        );
        screen.add(spaceship);  // muestra la nave en pantalla
    }

    // refresca la pantalla
    void redraw() {
        screen.refresh();
    }

    // dicta si el juego ya terminó
    bool isOver() {
        return gameOver;
    }
};
