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
#include <algorithm>
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
    int timer = 0; // contador de frames para el movimiento de los enemigos
	vector<SmallBird> small;
    vector<MutantBird> mutants;
	CollisionController collision;

public:
    EnemyController() {}

    void spawnSmall(int maxY, int maxX, int nivel, int tamanoP) {
        int y = 20;        
        int x = tamanoP-20;      // centrada horizontalmente
        int y1 = y;
        int x1 = x+32;
        if (nivel == 1){
            for (int i = 0; i <5; i++){
                y -= 1; 
                x -= 3;
                small.emplace_back(SmallBird(y, x));      
            }
            y -= 5;
            small.emplace_back(SmallBird(y, x));
            for (int i = 0; i <6; i++){
                y -= 1; 
                x += 3;
                small.emplace_back(SmallBird(y, x));   
            }
            for (int i = 0; i <5; i++){
                y1 -= 1; 
                x1 += 3;
                small.emplace_back(SmallBird(y1, x1));    
            }
            y1 -= 5;
            small.emplace_back(SmallBird(y1, x1));
            for (int i = 0; i <6; i++){
                y1 -= 1; 
                x1 -= 3;
                small.emplace_back(SmallBird(y1, x1));     
            }
        }
        y = 20;        
        x = tamanoP-10;      // centrada horizontalmente
        y1 = 20;
        x1 = tamanoP+5;

        if (nivel == 2){
            small.emplace_back(SmallBird(y, x));
            
            y += 6;

            for (int i = 0; i <4; i++){
                y -= 3; 
                x -= 4;
                small.emplace_back(SmallBird(y, x));  
            }
            for (int i = 0; i <3; i++){
                y += 3; 
                x -= 4;
                small.emplace_back(SmallBird(y, x));  
            }
            //2222222222222
            small.emplace_back(SmallBird(y1, x1));
            
            y1 += 6;

            for (int i = 0; i <4; i++){
                y1 -= 3; 
                x1 += 4;
                small.emplace_back(SmallBird(y1, x1));  
            }
            for (int i = 0; i <3; i++){
                y1 += 3; 
                x1 += 4;
                small.emplace_back(SmallBird(y1, x1));  
            }
        }
	
    }

    void spawnMutant(int maxY, int maxX, int nivel, int tamanoP) {
        int y = 20;        
        int x = tamanoP;      // centrada horizontalmente
        int y1 = y;
        int x1 = x+32;
        if (nivel == 3){
            for (int i = 0; i <5; i++){
                y += 4; 
                x -= 4;
                mutants.emplace_back(MutantBird(y, x));    
            }
        }
        y = 20;        
        x = tamanoP-20;      // centrada horizontalmente
        y1 = 10;
        x1 = tamanoP+5;
        if (nivel == 4){
            for (int i = 0; i <5; i++){
                y -= 1; 
                x -= 3;
                mutants.emplace_back(MutantBird(y, x));    
            }
            y -= 5;
            mutants.emplace_back(MutantBird(y, x));
            for (int i = 0; i <6; i++){
                y -= 1; 
                x += 3;
                mutants.emplace_back(MutantBird(y, x));   
            }
            ///////////////////////////////////////////////////////
            for (int i = 0; i <5; i++){
                y1 -= 1; 
                x1 += 3;
                mutants.emplace_back(MutantBird(y1, x1));    
            }
            y1 -= 5;
            mutants.emplace_back(MutantBird(y1, x1));
            for (int i = 0; i <6; i++){
                y1 -= 1; 
                x1 -= 3;
                mutants.emplace_back(MutantBird(y1, x1));     
            }
        }
        
	
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
            for (Bullet& bullet: bullets) {
                if (collision.checkCollision(spaceship, bullet))
			        return true;
            }
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
            // for (Bullet& bullet: bullets) {
            //     if (collision.checkCollision(spaceship, bullet))
			//         return true;
            // }
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
            // Si se eliminó, no incrementamos j porque los elementos se han desplazado
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
            // Si se eliminó, no incrementamos j porque los elementos se han desplazado
        }
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

        // Actualizar MutantBirds
        for (auto& enemy : mutants) {
            if (timer % 30 == 0) 
                enemy.update();
            if (timer % 90 == 0) 
                enemy.attack();
            for (auto& bullet : enemy.getBullets()) {
                bullet.move();
                screen.add(bullet);
            }
            screen.add(enemy);
        }
        timer++; // aumenta el frame actual
	}
};
