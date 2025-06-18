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
    SmallBird enemyBird;
    vector<Bullet> bullets;

public:
    MainLoop(int height, int width)
        : screen(height, width),
        gameOver(false),
        spaceship(0, 0)// valores temporales
    {
        screen.initialize();

        int birdY = 2;
        int birdX = screen.getWidth() / 2;
        enemyBird = SmallBird(birdY, birdX);

        int naveY = screen.getHeight() - 3; // 2 bloques arriba del borde inferior
        int naveX = screen.getWidth() / 2;  // centrada horizontalmente
        spaceship = Spaceship(naveY, naveX); // ahora sí, la nave queda bien posicionada
        curs_set(0); // Oculta cursor
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
            case ' ':                   // espacio es la tecla para disparar
                spaceship.setDirection(STAND);    
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


    void updateState() {
        screen.clear(); // limpia la pantalla para evitar residuos
        spaceship.move(screen.getWidth(), screen.getHeight()); // mueve la nave usando el ancho de la pantalla

        // Mueve y dibuja las balas activas
        for (Bullet& b : bullets) {
            b.move();
            screen.add(b);
        }

        // Elimina balas que se salen de la pantalla (por arriba)
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return b.getY() < 1; }),
            bullets.end()
        );

        screen.add(enemyBird);
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
