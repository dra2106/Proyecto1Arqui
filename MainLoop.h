#pragma once
#include <ncurses.h>
#include <stdexcept>

#include "Screen.h"
#include "Entity.h"
#include "SmallBird.h"
#include "Spaceship.h"
#include "Bullet.h"

#include "CollisionController.h"

using std::runtime_error;

class MainLoop {
private:
    Screen screen;
    bool gameOver;
    Spaceship spaceship;
    CollisionController collision;
    SmallBird bird;
    Bullet bullet;

public:
    MainLoop(int height, int width)
        : screen(height, width),
          gameOver(false),
          spaceship(25, 16),
          bird(25, 20)
    {
        screen.initialize();
        curs_set(0);           // Oculta cursor
    }

    // procesa el input del usuario
    void processInput() {
        chtype input = screen.getInput();
        switch (input) {
            case KEY_UP: // por ahora no hace nada, IMPLEMENTAR ESCUDO
                bullet = Bullet(spaceship.getY() - 1, spaceship.getX());
                screen.add(bullet);
                bullet.setDirection(up);
                break;
            case KEY_DOWN:
                spaceship.setDirection(stand);
                break;  // no hace nada
            case KEY_LEFT:
                spaceship.setDirection(left);
                break;
            case KEY_RIGHT:
                spaceship.setDirection(right);
                break;
            default:
                spaceship.setDirection(stand);
                break;
        }

        if (collision.checkCollision(spaceship, bird))
            throw runtime_error("SE MATO!!!!!!");
    }

    // actualiza el estado de la pantalla
    void updateState() {
        screen.clear(); // limpia la pantalla para evitar residuos

        spaceship.move(); // mueve la nave
        screen.add(bird);
        bullet.move();
        screen.add(SmallBird(5, 15));
        screen.add(spaceship); // muestra la nave en pantalla
        screen.add(bullet);
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
