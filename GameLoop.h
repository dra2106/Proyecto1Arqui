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

class GameLoop {
private:
    Screen screen;
    Spaceship spaceship;
    CollisionController collision;
    EnemyController enemyCon;
    vector<Bullet> bullets;
    vector<SmallBird> smallBirds;

public:
    GameLoop(int height, int width)
        : screen(height, width),
        spaceship(0, 0)
    {
        screen.initialize();
        for (int i = 0; i < 5; i++)
            enemyCon.spawnEnemy(screen.getWidth(), screen.getHeight());

        srand(time(0));
        int naveY = screen.getHeight() - 3;     
        int naveX = screen.getWidth() / 2;    
        spaceship = Spaceship(naveY, naveX);  
        curs_set(0);                           
    }

    void processInput() {
        chtype input = screen.getInput();
        switch (input) {
            case KEY_UP:            
                spaceship.setDirection(STAND);    
                break;
            case KEY_LEFT:
                spaceship.setDirection(LEFT);
                break;
            case KEY_RIGHT:
                spaceship.setDirection(RIGHT);
                break;
            case ' ':
                spaceship.setDirection(STAND);    
                if (bullets.size() < 5) {    
                    bullets.emplace_back(spaceship.getY() - 1, spaceship.getX());
                    for (Bullet& b : bullets) {
                        b.setDirection(UP);   
                    }
                }
                break;
            default:
                spaceship.setDirection(STAND);
                break;
        }
    }
    void updateState() {
        screen.clear();         
        spaceship.move(screen.getWidth(), screen.getHeight()); 
        if (enemyCon.checkPlayerCollisions(spaceship))
            throw runtime_error("Game Over: Collision with enemy!");

        enemyCon.updateAll(screen); 
        
        for (Bullet& b : bullets) {
            b.move();
            screen.add(b);
        }
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return b.getY() < 1; }),
            bullets.end()
        );
        screen.add(spaceship);
    }

    void redraw() {
        screen.refresh();
    }

};
