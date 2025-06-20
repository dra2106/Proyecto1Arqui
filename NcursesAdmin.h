#pragma once
#include <string>
#include <vector>
#include <ncurses.h>

#include "Screen.h"
#include "DataStructures/DLinkedList.h"
#include "DataStructures/KVPair.h"
#include "ScreenType.h"

class NcursesAdmin {
private:
    int screenWidth;
    int screenHeight;
    Screen* screen; // Pointer to Screen

public:
    NcursesAdmin() {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
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
        screen->refresh();
        endwin(); // End ncurses mode
    }

    void clearScreen() {
        screen->clear();
        screen->refresh();
    }

    void showCenteredString(const std::string& text, int row) {
        int col = (screenWidth - text.size()) / 2;
        mvwprintw(stdscr, row, col, "%s", text.c_str());
        refresh();
    }

    void showStringAt(int y, int x, const std::string& text) {
        mvwprintw(stdscr, y, x, "%s", text.c_str());
        refresh();
    }

    void setScreenFormat();

    void showScreen(ScreenType type, 
                    const std::string& name = "", 
                    int score = 0, 
                    int highestScore = 0, 
                    int level = 0, 
                    int lives = 0, 
                    const std::vector<std::string>& highscores = {})
    {
        screen->clear();
        screen->addBorder();
        switch (type) {
            case ScreenType::START: {
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
            case ScreenType::GAME: {
                screen->addBorder();

                // Primera fila (centrada arriba)
                int row1 = 2;
                std::string label1 = "Name: " + name;
                std::string label2 = "Score: " + std::to_string(score);
                std::string label3 = "Level: " + std::to_string(level);

                // Calcula el ancho total de las 3 etiquetas y los espacios entre ellas
                int space = 6;
                int totalWidth = label1.size() + label2.size() + label3.size() + 2 * space;
                int startCol = (screenWidth - totalWidth) / 2;

                screen->showStringAt(label1, row1, startCol);
                screen->showStringAt(label2, row1, startCol + label1.size() + space);
                screen->showStringAt(label3, row1, startCol + label1.size() + space + label2.size() + space);

                // Segunda fila (debajo, centrada)
                int row2 = row1 + 2;
                std::string label4 = "Lives: " + std::to_string(lives);
                std::string label5 = "Press Q to quit";
                int totalWidth2 = label4.size() + space + label5.size();
                int startCol2 = (screenWidth - totalWidth2) / 2;

                screen->showStringAt(label4, row2, startCol2);
                screen->showStringAt(label5, row2, startCol2 + label4.size() + space);

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
                screen->showCenteredString("Game Over");
                screen->showCenteredString("Name: " + name);
                screen->showCenteredString("Score: " + std::to_string(score));
                screen->showCenteredString("Highest Score: " + std::to_string(highestScore));
                screen->showCenteredString("Level: " + std::to_string(level));
                screen->showCenteredString("Lives Remaining: " + std::to_string(lives));
                screen->showCenteredString("Press any key to continue...");
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

    void refresh() { screen->refresh(); }

    Screen getScreen() const {
        return *screen;
    }
};