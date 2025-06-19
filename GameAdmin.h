#pragma once
#include <string>
#include "NcursesAdmin.h"
#include "ScoreController.h"

using std::string;

class GameAdmin {
public:
    GameAdmin() : ncurses(), scoreController("Files/Scores.txt", "Jugador") {
        // Inicializa los atributos del juego
        nombreJugador = "Jugador";
        scoreActual = 0;
        highestScore = 0;
        nivel = 1;
        vidasRestantes = 3;
        juegoTerminado = false;
    }
    ~GameAdmin(){
        // Finaliza la pantalla ncurses
        ncurses.finalizarPantalla();
    }

    void iniciarJuego(){
        // Inicializa la pantalla ncurses
        ncurses.inicializarPantalla();

        // Muestra la pantalla de inicio (Titulo del juego)
        ncurses.mostrarPantalla(Pantalla::INICIO); // Espera cualquier tecla para continuar

        // Muestra la pantalla de solicitud de nombre y lo obtiene
        ncurses.mostrarPantalla(Pantalla::SOLICITUD_NOMBRE, nombreJugador);
        nombreJugador = ncurses.solicitarInput("Ingrese su nombre: ");

        // Bucle principal del juego
        mainLoop();
        
        // Al finalizar el juego, guarda el puntaje del jugador
        scoreController.setPlayerName(nombreJugador);
        scoreController.setPlayerScore(scoreActual);
        scoreController.saveGame();
        
        // Carga los puntajes más altos desde el archivo
        scoreController.loadGame();
        
        DLinkedList<KVPair<string, int>> highScoresList;
        scoreController.getScores(highScoresList);
        
        // Al finalizar el juego, muestra la pantalla de Game Over
        ncurses.mostrarPantalla(Pantalla::GAME_OVER, nombreJugador, scoreActual, highestScore, nivel, vidasRestantes);

        // Muestra los puntajes más altos
        ncurses.mostrarPantalla(Pantalla::HIGHSCORES, highScoresList);
    }

    bool deseaContinuar() {
        ncurses.limpiarPantalla();
        char respuesta = ncurses.solicitarInput("¿Desea continuar jugando? (s/n): ")[0];
        
        if (respuesta == 's' || respuesta == 'S')
            return true; // Continúa el juego
        else
            return false; // Cierra el juego
    }
    
private:
    void mainLoop(){
        while (!juegoTerminado) {
            ncurses.limpiarPantalla();
            ncurses.mostrarPantalla(Pantalla::JUEGO, nombreJugador, scoreActual, highestScore, nivel, vidasRestantes);

            procesarInputs();
            updateEstado();
            redraw();
            revisarColisiones();
            revisarFinJuego();

            juegoTerminado = (vidasRestantes <= 0);
        }
    }

    void procesarInputs(){
        // Procesa la entrada del usuario
    }
    void updateEstado();
    void redraw();
    void revisarColisiones();
    void revisarFinJuego();

    NcursesAdmin ncurses;
    ScoreController scoreController;

    string nombreJugador;
    int scoreActual;
    int highestScore;
    int nivel;
    int vidasRestantes;
    bool juegoTerminado;
};