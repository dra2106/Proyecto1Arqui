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

#include <iostream>
#include <string>
#include <cstring>
#include <ncurses.h>
#include "Spaceship.h"
#include "MainLoop.h"
#include "Entity.h"
#include "ScoreController.h"
#include "DataStructures/DLinkedList.h"
#include "DataStructures/KVPair.h"

using std::cout;
using std::endl;

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, FALSE); // Espera por una tecla
    refresh();

    const char* titulo[] = {
        "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@=:::::%@@@@@@@%:%@@@@@@@%:::::-:%@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@-::::::::::::@@@@-:::-@@@@::::::::::::::#@@@@@@@@@@@@@",
        "@@@@@@@@@#**-::::::::::::::::=*:::::::*=::::::::::::::::::+#@@@@@@@@@",
        "@@@@@@@@--::::::::::::::::::::-###:*##-::::::::::::::::::::--@@@@@@@@",
        "@@@@@@+::::::::::-@=-@=:::::::-###:###-::::::@@@--@-::::::::::=@@@@@@",
        "@@@@@:::::::-=@=*@@@@@+=@=-::::=======::::-=@@@@@@@=#@-%@--:::::#@@@@",
        "@@@#=::::+*:%@@@@@@@@@@@@@@#-:-#######-::#@@@@@@@@@@@@@@@@%*:::::+@@@",
        "@@@*::*%:#@@@@@@@@@@@@@@@@@@-:::#####:::-@@@@@@@@@@@@@@@@@@#:%#::+@@@",
        "@@@*-@@@@@@@@@@@@@@@@@@@@@@@@+:::=#+:::+@@@@@@@@@@@@@@@@@@@@@@@@-+@@@",
        "@@@%*@@@@@@@@@@@@@@@@@@@@@%+:-*@*+:+*@*-:+%@@@@@@@@@@@@@@@@@@@@@*%@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@::::+@@@@@@@@@*::::@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@:=@:+@@@@@@@@@*:@=:@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@- =**+.+%  %@@- +#-.**+ =@: +****#% .=@@: #@%*+ :**@: %@@+ -@@@@@",
        "@@@@- *@@@ :%  +**: +* :@@@  %: +***#@% .=.+: #@@@# :@@@@+.+::*@@@@@@",
        "@@@@- ....:@%  %@@- +* :@@@  %: #@@@@@% .@@:  #@@@# :@@@@:.%-.=@@@@@@",
        "@@@@- *@@@@@%  %@@- +@#.::: %@: .::::-% .@@@: #@*:. .::@: %@@+ -@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
    };
    int num_lines = sizeof(titulo) / sizeof(titulo[0]);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    clear();
    for (int i = 0; i < num_lines; ++i) {
        int len = strlen(titulo[i]);
        int start_col = (cols - len) / 2;
        int start_row = (rows - num_lines) / 2;
        mvprintw(start_row + i, start_col, "%s", titulo[i]);
    }
    refresh();
    getch(); // Espera a que el usuario presione cualquier tecla

    clear();
    refresh();
    nodelay(stdscr, TRUE); // Vuelve a modo no bloqueante
    getch();
    clear();

    MainLoop game(rows - 2, cols/3 - 4);
    while (!game.isOver()) {
        game.processInput();
        game.updateState();
        game.redraw();
        napms(32);
    }
    getch();
    endwin();
    cout << "Has perdido!!" << endl;
    // Mostrar highscores
    ScoreController scoreController("Files/Scores.txt", "JUGADOR"); // Cambia el nombre si lo pides antes
    DLinkedList<KVPair<std::string, int>> scores;
    scoreController.getScores(scores);

    cout << "\nHIGHSCORES:\n";
    for (scores.goToStart(); !scores.atEnd(); scores.next()) {
        auto pair = scores.getElement();
        cout << pair.key << " - " << pair.value << " PTS" << endl;
    }
    return 0;
}
