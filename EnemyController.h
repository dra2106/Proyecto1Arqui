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
        srand(time(0));
    }

    void spawnEnemy(int maxY, int maxX) {
		int y = rand() % (maxY / 3) + 1;
        int x = rand() % (maxX - 2) + 1;
        small.emplace_back(SmallBird(y, x));
	}

	bool checkPlayerCollisions(const Spaceship& spaceship){
		for (SmallBird& enemy: small) {
			if (collision.checkCollision(spaceship, enemy))
			    return true;
            vector<Bullet> bullets = enemy.getBullets();
            bullets.erase(
                std::remove_if(bullets.begin(), bullets.end(),
                    [](const Bullet& b) { return b.getY() > 50; }),
                bullets.end()
            );
            for (Bullet& bullet: bullets) {
                if (collision.checkCollision(spaceship, bullet))
			        return true;
            }
		}
		return false;
	}

	void updateAll(Screen& screen) {
		for (SmallBird& enemyBird : small) {
            if (timer % 30 == 0) {
                enemyBird.update();
            }
            if (timer % 90 == 0) { // cada 120 frames, el pájaro ataca
                enemyBird.attack();
            }
            for (Bullet& b : enemyBird.getBullets()) {
                b.move();
                screen.add(b);
            }
            screen.add(enemyBird);
        }
        timer++; // aumenta el frame actual
	}
};

