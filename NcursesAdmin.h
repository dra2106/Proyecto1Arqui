#pragma once
#include <string>
#include <vector>
#include <ncurses.h>

#include "Screen.h"
#include "DataStructures/DLinkedList.h"
#include "DataStructures/KVPair.h"
#include "ScreenType.h"
#include "Profiler.h"

class NcursesAdmin {
private:
    bool gameWindowInitialized; // Flag to check if the screen is initialized
    int screenWidth;
    int screenHeight;
    Screen* screen; // Pointer to Screen

public:
    NcursesAdmin() {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        gameWindowInitialized = false;
        getmaxyx(stdscr, screenHeight, screenWidth);
        screen = new Screen(screenHeight, screenWidth); // Allocate Screen
    }
    ~NcursesAdmin() {
        delete screen; // Free memory
        endwin();
    }

    void initializeScreen(){
        screen->initialize();
        curs_set(0); // Hide the cursor
    }

    void finalizeScreen(){
        screen->clear();
        // screen->refresh();
        endwin(); // End ncurses mode
    }

    void clearScreen() {
        screen->clear();
    }

    void showCenteredString(const std::string& text, int row) {
        int col = (screenWidth - text.size()) / 2;
        mvwprintw(stdscr, row, col, "%s", text.c_str());
        // refresh();
    }

    void showStringAt(int y, int x, const std::string& text) {
        mvwprintw(stdscr, y, x, "%s", text.c_str());
        // refresh();
    }

    void setScreenFormat();

    void showScreen(ScreenType type, 
                    const std::string& name = "", 
                    int score = 0, 
                    int highestScore = 0, 
                    int level = 1, 
                    int lives = 0, 
                    const std::vector<std::string>& highscores = {},
                    const Profiler* profiler = nullptr)
    {
        
        switch (type) {
            case ScreenType::START: {
                screen->clear();
                screen->addBorder();
                const char* title[] = {
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
                int numLines = sizeof(title) / sizeof(title[0]);
                int startRow = (screenHeight - numLines) / 2;
                for (int i = 0; i < numLines; ++i) {
                    showCenteredString(title[i], startRow + i);
                }
                showCenteredString("Press any key to start...", startRow + numLines + 10);
                getch(); // Wait for any key
                break;
            }
            case ScreenType::NAME_REQUEST: {
                screen->clear();
                break;
            }
            case ScreenType::GAME:
            case ScreenType::GAME_TEST: {
                screen->clear();

                int row1 = 2;
                int startCol = 2;

                std::string label1 = "Name: " + name;
                std::string label2 = "Score: " + std::to_string(score);
                std::string label3 = "Level: " + std::to_string(level);

                int space = 6;

                screen->showStringAt(label1, row1, startCol);
                screen->showStringAt(label2, row1, startCol + label1.size() + space);
                screen->showStringAt(label3, row1, startCol + label1.size() + space + label2.size() + space);

                // Segunda fila (alineados a la derecha, uno a la izquierda del otro, margen de 4)
                int row2 = row1 + 2;
                int spaceRight = 15;
                int rightMargin = 4;

                std::string label4 = "Highest Score: " + std::to_string(highestScore);
                std::string label5 = "Lives: " + std::to_string(lives);
                std::string label6 = "Press Q to quit";

                int totalWidth = label4.size() + spaceRight + label5.size() + spaceRight + label6.size();
                int startColRight = screenWidth - totalWidth - rightMargin;

                screen->showStringAt(label4, row2, startColRight);
                screen->showStringAt(label5, row2, startColRight + label4.size() + spaceRight);
                screen->showStringAt(label6, row2, startColRight + label4.size() + spaceRight + label5.size() + spaceRight);

                // Si es la pantalla de métricas, muestra las métricas del profiler
                if (type == ScreenType::GAME_TEST && profiler != nullptr) {
                    // Mostrar métricas en la parte inferior de la pantalla
                    int metricsHeight = 15; // Altura de la ventana de métricas
                    int metricsStartRow = screenHeight - metricsHeight - 2; // Cerca del fondo de la pantalla
                    int metricsStartCol = 2; // Alineado a la izquierda con margen
                    
                    // Asegurar que no salga de la pantalla
                    if (metricsStartRow < row2 + 5) {
                        metricsStartRow = row2 + 5; // Mínimo 5 filas debajo de los labels
                    }
                    
                    // Crear una ventana para las métricas
                    WINDOW* metricsWin = newwin(metricsHeight, 65, metricsStartRow, metricsStartCol);
                    if (metricsWin != nullptr) {
                        wborder(metricsWin, '|', '|', '-', '-', '+', '+', '+', '+');
                        mvwprintw(metricsWin, 0, 2, " AMDAHL PERFORMANCE METRICS ");
                        profiler->drawSummary(metricsWin, 1, 1);
                        wrefresh(metricsWin);
                        delwin(metricsWin);
                    }
                }
                break;
            }

            case ScreenType::HIGHSCORES: {
                screen->clear();
                screen->addBorder();
                screen->showStringAt("High Scores", 1, (screenWidth - std::string("High Scores").size()) / 2);
                int row = 3;
                for (const auto& highscore : highscores) {
                    screen->showStringAt(highscore, row, (screenWidth - highscore.size()) / 2);
                    ++row;
                }
                getch(); // Wait for any key
                break;
            }

            case ScreenType::GAME_OVER: {
                screen->clear();
                screen->addBorder();
                screen->showStringAt("Game Over", 1, (screenWidth - std::string("Game Over").size()) / 2);
                screen->showStringAt("Name: " + name, 3, (screenWidth - ("Name: " + name).size()) / 2);
                screen->showStringAt("Score: " + std::to_string(score), 4, (screenWidth - std::string("Score: " + std::to_string(score)).size()) / 2);
                screen->showStringAt("Highest Score: " + std::to_string(highestScore), 5, (screenWidth - std::string("Highest Score: " + std::to_string(highestScore)).size()) / 2);
                screen->showStringAt("Level: " + std::to_string(level), 6, (screenWidth - std::string("Level: " + std::to_string(level)).size()) / 2);
                screen->showStringAt("Lives Remaining: " + std::to_string(lives), 7, (screenWidth - std::string("Lives Remaining: " + std::to_string(lives)).size()) / 2);
                screen->showStringAt("Press any key to continue...", 8, (screenWidth - std::string("Press any key to continue...").size()) / 2);
                getch(); // Wait for any key
                break;
            }

            case ScreenType::CONTINUE_REQUEST: {
                screen->clear();
                break;
            }
            
            default:
                break;
        }
        screen->refresh();
    }

    std::string requestInput(const std::string& message) {
        screen->showCenteredString(message);
        int inputRow = (screenHeight / 2) + 2;
        int inputCol = (screenWidth - 30) / 2; 
        move(inputRow, inputCol);
        char input[256];
        echo();
        mvgetnstr(inputRow, inputCol, input, sizeof(input) - 1);
        noecho();
        input[sizeof(input) - 1] = '\0';
        return std::string(input);
    }

    chtype getInput() {
        return screen->getInput();
    }

    int getScreenWidth() { return screenWidth; }

    int getScreenHeight() { return screenHeight; }

    void setScreenFormat(int height, int width) {
        screen->clear();
        screen->initialize();
        screen->construct(height, width);
    }

    void refresh() { screen->refresh(); }

    Screen getScreen() const {
        return *screen;
    }

    void setFalseGameWindowInitialized() {
        gameWindowInitialized = false;
        screen->clear();
        setScreenFormat(screenHeight, screenWidth * 3);
    }
};