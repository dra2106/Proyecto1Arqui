#pragma once

#include <ncurses.h>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "Screen.h"
#include "Entity.h"
#include "SmallBird.h"
#include "Spaceship.h"
#include "Bullet.h"

#include "CollisionController.h"

using std::runtime_error;
using std::vector;

class MainLoop {
private:
    Screen screen;
    bool gameOver;
    Spaceship spaceship;
    CollisionController collision;
    SmallBird bird;
    Bullet bullet;
    vector<Bullet> bullets;

public:
    MainLoop(int height, int width)
        : screen(height, width),
          gameOver(false),
          bird(5, 15)
    {
        screen.initialize();
        spaceship = Spaceship(screen, 7, 15);
        curs_set(0);           // Oculta cursor
    }

    // procesa el input del usuario
    void processInput() {
        chtype input = screen.getInput();
        switch (input) {
            case KEY_UP:                // por ahora no hace nada, IMPLEMENTAR ESCUDO
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
            case ' ':                   // espacio es la tecla para disparar
                if (bullets.size() < 5) // límite de 5 balas en pantalla
                bullets.emplace_back(spaceship.getY() - 1, spaceship.getX());
                // bullet = Bullet(spaceship.getY() - 1, spaceship.getX());
                // bullet.setDirection(up);
                // bullets.push_back(bullet);
                break;
            default:
                spaceship.setDirection(STAND);
                break;
        }

        // si hubo una colisión, termina el juego
        //if (collision.checkCollision(spaceship, bird))
          //  gameOver = true;

        // idea para eliminar aves cuando se les dispara
        // for (Bullet& b : bullets){
        //     if (collision.checkCollision(b, bird))
        //         bird.~SmallBird();
        // }
    }

    // actualiza el estado de la pantalla
    void updateState() {
        screen.clear(); // limpia la pantalla para evitar residuos
        spaceship.move(); // mueve la nave
        screen.add(bird);

        // for para mover y actualizar el movimiento de las balas
        for (Bullet& b : bullets){
            screen.add(b);
            b.move();
        }

        // este desmadre elimina balas si se salen de la pantalla
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(), 
                [](const Bullet& b) { return b.getY() < 1; }),
            bullets.end()
        );
        
        screen.add(bird);
        screen.add(spaceship); // muestra la nave en pantalla
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
