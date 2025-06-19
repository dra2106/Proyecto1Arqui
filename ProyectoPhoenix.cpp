/* 
 * Nombre de Archivo: Proyecto1Arqui.cpp
 *
 * Descripcion:
 * Este archivo contiene la función principal del juego. Inicializa la pantalla,
 * muestra la pantalla de inicio, ejecuta el ciclo principal del juego y, al finalizar,
 * muestra los puntajes más altos (highscores) obtenidos desde el archivo correspondiente.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#include "GameAdmin.h"

int main() {
    GameAdmin game;
    bool continuar = true;
    while (continuar) {
        game.iniciarJuego();
        continuar = game.deseaContinuar();
    }
    return 0;
}