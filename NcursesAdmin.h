#pragma once
#include <string>
#include <vector>
#include <ncurses.h>

#include "DataStructures/DLinkedList.h"
#include "DataStructures/KVPair.h"

enum class Pantalla {
    INICIO,
    SOLICITUD_NOMBRE,
    JUEGO,
    HIGHSCORES,
    GAME_OVER,
    SOLICITUD_CONTINUAR
};

class NcursesAdmin {
public:
    NcursesAdmin();
    ~NcursesAdmin();

    void inicializarPantalla();
    void finalizarPantalla();

    void limpiarPantalla();

    void mostrarCadenaCentrada(const std::string& texto, int fila);
    void mostrarCadenaEn(int y, int x, const std::string& texto);

    void setearFormatoPantalla();
    void mostrarPantalla(Pantalla tipo, 
                         const std::string& nombre = "", 
                         int score = 0, 
                         int highestScore = 0, 
                         int nivel = 0, 
                         int vidas = 0, 
                         const std::vector<std::string>& highscores = {});

    void mostrarPantalla(Pantalla tipo, const DLinkedList<KVPair<string, int>>& highscores);

    std::string solicitarInput(const std::string& mensaje);

private:
    int anchoPantalla;
    int altoPantalla;
    WINDOW* mainwin;
};