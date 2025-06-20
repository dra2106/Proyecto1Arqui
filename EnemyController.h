/* 
 * Nombre de Archivo: MainLoop.h
 *
 * Descripcion:
 * Este archivo define la clase MainLoop, que controla el ciclo principal del juego,
 * gestionando la entrada del usuario, la actualización de entidades, el control de colisiones,
 * y la lógica de avance de niveles y finalización del juego.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include <vector>
#include <time.h>

#include "Screen.h"
#include "CollisionController.h"
#include "Enemy.h"
#include "SmallBird.h"
#include "MutantBird.h"
#include "Mothership.h"
#include "Bullet.h"

using std::vector;

class EnemyController {
private:
	vector<SmallBird> small;
    vector<MutantBird> mutants;
    vector<Mothership> mother;
	CollisionController collision;

public:
    EnemyController() {}

    void spawnSmall(int maxY, int maxX) {
		int y = rand() % (maxY / 3) + 1;        // se divide entre 3 para que no salga muy cerca del jugador
        int x = rand() % (maxX - 2) + 1;
        small.emplace_back(SmallBird(y, x));    // se añade a la lista de enemigos
	}

    void spawnMutant(int maxY, int maxX) {
		int y = rand() % (maxY / 3) + 1;        // se divide entre 3 para que no salga muy cerca del jugador
        int x = rand() % (maxX - 10) + 5;
        mutants.emplace_back(MutantBird(y, x));    // se añade a la lista de enemigos
	}

    void spawnMother(int maxY, int maxX) {
		int y = maxY / 3;        // se divide entre 3 para que no salga muy cerca del jugador
        int x = maxX - 30;
        mother.emplace_back(Mothership(y, x));    // se añade a la lista de enemigos
	}

	bool checkPlayerCollisions(const Spaceship& spaceship){
        // por cada enemigo, si toca al jugador, pierde
		for (SmallBird& enemy: small) {
			if (collision.checkCollision(spaceship, enemy))
			    return true;
            // propenso a cambio, elimina balas que hayan pasado cierta altura (actualmente se borran, pero el sprite sigue)
            vector<Bullet> bullets = enemy.getBullets();
            bullets.erase(
                std::remove_if(bullets.begin(), bullets.end(),
                    [](const Bullet& b) { return b.getY() > 50; }),
                bullets.end()
            );
            // colisión de la nave con las balas
            for (int i = 0; i < bullets.size(); i++) {
                if (collision.checkCollision(spaceship, bullets[i])){
                    bullets.erase(bullets.begin() + i);
                    return true;
                }
            }
            // for (Bullet& bullet: bullets) {
            //     if (collision.checkCollision(spaceship, bullet))
			//         return true;
            // }
		}
        for (MutantBird& enemy: mutants) {
			if (collision.checkCollision(spaceship, enemy))
			    return true;
            vector<Bullet> bullets = enemy.getBullets();
            bullets.erase(
                std::remove_if(bullets.begin(), bullets.end(),
                    [](const Bullet& b) { return b.getY() > 50; }),
                bullets.end()
            );
            for (int i = 0; i < bullets.size(); i++) {
                if (collision.checkCollision(spaceship, bullets[i])){
                    bullets.erase(bullets.begin() + i);
                    return true;
                }
            }
		}

        for (Mothership& enemy: mother) {
			if (collision.checkCollision(spaceship, enemy))
			    return true;
            vector<Bullet> bullets = enemy.getBullets();
            bullets.erase(
                std::remove_if(bullets.begin(), bullets.end(),
                    [](const Bullet& b) { return b.getY() > 50; }),
                bullets.end()
            );
            for (int i = 0; i < bullets.size(); i++) {
                if (collision.checkCollision(spaceship, bullets[i])){
                    bullets.erase(bullets.begin() + i);
                    return true;
                }
            }
		}
		return false;
	}

    void CheckCollisionsEnemies(vector<Bullet>& Bullets)
    {
        for (int j = 0; j < small.size(); )
        {
            bool erased = false;
            for (int i = 0; i < Bullets.size(); )
            {
                if (collision.checkCollision(small[j], Bullets[i])) {
                    if (small[j].getHealth() < 1) {
                        small.erase(small.begin() + j); // Elimina el enemigo
                        erased = true;
                        break; // Salir del bucle de balas, ya que el enemigo fue eliminado
                    }
                    small[j].damage();
                    Bullets.erase(Bullets.begin() + i); // Elimina la bala
                } else {
                    ++i;
                }
            }
            if (!erased) {
                ++j;
            }
        }
        for (int j = 0; j < mutants.size(); )
        {
            bool erased = false;
            for (int i = 0; i < Bullets.size(); )
            {
                if (collision.checkCollision(mutants[j], Bullets[i])) {
                    if (mutants[j].getHealth() < 1) {
                        mutants.erase(mutants.begin() + j); // Elimina el enemigo
                        erased = true;
                        break; // Salir del bucle de balas, ya que el enemigo fue eliminado
                    }
                    mutants[j].damage();
                    Bullets.erase(Bullets.begin() + i); // Elimina la bala
                } else {
                    ++i;
                }
            }
            if (!erased) {
                ++j;
            }
        }

        for (int j = 0; j < mother.size(); )
        {
            bool erased = false;
            for (int i = 0; i < Bullets.size(); )
            {
                if (collision.checkCollision(mother[j], Bullets[i])) {
                    if (mother[j].getHealth() < 1) {
                        mother.erase(mother.begin() + j); // Elimina el enemigo
                        erased = true;
                        break; // Salir del bucle de balas, ya que el enemigo fue eliminado
                    }
                    mother[j].damage();
                    Bullets.erase(Bullets.begin() + i); // Elimina la bala
                } else {
                    ++i;
                }
            }
            if (!erased) {
                ++j;
            }
        }
    }

    // actualiza todos los enemigos en pantalla y sus balas
    // controla los patrones de ataque por medio de frames
	void updateAll(Screen& screen) {
		for (SmallBird& enemyBird : small) {
            enemyBird.update();
            
            for (Bullet& b : enemyBird.getBullets()) {
                b.move();
                screen.add(b);          // actualiza balas
            }
            screen.add(enemyBird);      // actualiza enemigos
        }

        // Actualizar MutantBirds
        for (auto& enemy : mutants) {
            enemy.update();
            
            for (auto& bullet : enemy.getBullets()) {
                bullet.move();
                screen.add(bullet);
            }
            screen.add(enemy);
        }

        for (auto& mother : mother) {
            mother.update();
            for (auto& bullet : mother.getBullets()) {
                bullet.move();
                screen.add(bullet);
            }
            screen.add(mother);
        }
	}
};
