#pragma once

#include <vector>
#include "Enemy.h"
#include "SmallBird.h"
#include "Bullet.h"
#include <time.h>
#include "Screen.h"
#include "CollisionController.h"

using std::vector;

class EnemyController {
private:
    int timer = 0; // contador de frames para el movimiento de los enemigos
	vector<SmallBird> small;
	CollisionController collision;

public: 
    EnemyController() {
        srand(time(0)); // genera la semilla para las posiciones
    }

    void spawnEnemy(int maxY, int maxX) {
		int y = rand() % (maxY / 3) + 1;        // se divide entre 3 para que no salga muy cerca del jugador
        int x = rand() % (maxX - 2) + 1;
        small.emplace_back(SmallBird(y, x));    // se añade a la lista de enemigos
	}

    // revisa las colisiones con la nave
	bool checkPlayerCollisions(const Spaceship& spaceship){
        // por cada enemido, si toca al jugador, pierde
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
            for (Bullet& bullet: bullets) {
                if (collision.checkCollision(spaceship, bullet))
			        return true;
            }
		}
		return false;
	}

    // actualiza todos los enemigos en pantalla y sus balas
    // controla los patrones de ataque por medio de frames
	void updateAll(Screen& screen) {
		for (SmallBird& enemyBird : small) {
            if (timer % 30 == 0) {      // se mueve cada 30 frames (1 segundo)
                enemyBird.update();
            }
            if (timer % 90 == 0) {      // dispara cada 90 frames (3 segundos)
                enemyBird.attack();
            }
            for (Bullet& b : enemyBird.getBullets()) {
                b.move();
                screen.add(b);          // actualiza balas
            }
            screen.add(enemyBird);      // actualiza enemigos
        }
        timer++; // aumenta el frame actual
	}
};

