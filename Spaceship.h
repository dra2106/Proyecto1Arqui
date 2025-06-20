/* 
 * Nombre de Archivo: Spaceship.h
 *
 * Descripcion:
 * Este archivo define la clase Spaceship, que representa la nave controlada por el jugador.
 * Incluye la lógica para el movimiento, la selección y actualización del sprite según la dirección,
 * y la gestión de la posición dentro de los límites de la pantalla.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#include "Entity.h"
#include "DataStructures/DLinkedList.h"

class Spaceship : public Entity {
private:
    Direction currentDirection;
    int shieldFrames = 0;
    int SHIELD_DURATION = 60;     // 2 segundos activo
    int SHIELD_COOLDOWN = 300;    // 10 segundos de cooldown

    void loadSprite() {
        DLinkedList<string> sprite;
        string shieldIndicator;
        
        // imprime un indicador de que el escudo está activo
        if (shieldFrames > 0) {
            sprite.append(" ___ ");
            sprite.append("/   \\");

        } else if (shieldFrames < 0) {
            int cooldown = -shieldFrames / 30;
            shieldIndicator = "(" + std::to_string(cooldown) + ")"; // Tiempo de cooldown

        } else {
            shieldIndicator = "[!]"; // Escudo listo
        }
        switch (currentDirection) {
            case LEFT:
                sprite.append("(\\A/|");
                sprite.append("(!H!|");
                sprite.append("(/V\\|");
                sprite.append(" " + shieldIndicator);
                break;
            case RIGHT:
                sprite.append("|\\A/)");
                sprite.append("|!H!)");
                sprite.append("|/V\\)");
                sprite.append(" " + shieldIndicator);
                break;
            case STAND:
            default:
                sprite.append("|\\A/|");
                sprite.append("|!H!|");
                sprite.append("|/V\\|");
                sprite.append(" " + shieldIndicator);
                break;
        }
        setSprite(sprite);
    }

public:
    Spaceship(int y, int x) : Entity(x, y) {
        currentDirection = Direction::STAND;
        loadSprite();
    }

    void setDirection(Direction d) {
        currentDirection = d;
        loadSprite(); // Actualiza el sprite al cambiar la dirección
    }

    void activateShield() {
        if (shieldFrames == 0) { // Solo activar si está listo
            shieldFrames = SHIELD_DURATION;
            loadSprite();
        }
    }

    void update() {
        if (shieldFrames > 0) { // si se activa el escudo, reduce los frames
            shieldFrames--;
            if (shieldFrames == 0) {
                shieldFrames = -SHIELD_COOLDOWN; // Iniciar cooldown
                loadSprite();
            }
        } else if (shieldFrames < 0) { // si está en cooldown, aumenta los frames
            shieldFrames++;
            if (shieldFrames == 0) {
                loadSprite(); // Escudo listo otra vez
            }
        }
    }

    bool isShieldActive() { 
        return shieldFrames > 0;
    }

    Direction getDirection() { 
        return currentDirection;
    }

    void move(int screenWidth, int screenHeight) {
        const int margenX = 1;

        switch (currentDirection) {
            case LEFT:  x--; break;
            case RIGHT: x++; break;
            case UP:    y--; break;
            case DOWN:  y++; break;
            default:    break;
        }

        // Calcula el ancho real del sprite (la línea más larga)
        int spriteWidth = 0;
        DLinkedList<std::string> spr = getSprite();
        for (spr.goToStart(); !spr.atEnd(); spr.next()) {
            int lineWidth = spr.getElement().size();
            if (lineWidth > spriteWidth) spriteWidth = lineWidth;
        }

        if (x < margenX) x = margenX;
        if (x < margenX + spriteWidth / 2) x = margenX + spriteWidth / 2;
        if (x > screenWidth - spriteWidth - margenX)
            x = screenWidth - spriteWidth - margenX;
    }
};