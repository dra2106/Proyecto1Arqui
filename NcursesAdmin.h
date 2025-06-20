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
                showCenteredString("Press any key to start...", startRow + numLines + 2);
                getch(); // Wait for any key
                break;
            }
            // Implement other cases as needed...
            default:
                break;
        }
        screen->refresh();
    }

    void showScreen(ScreenType type, const DLinkedList<KVPair<std::string, int>>& highscores);

    std::string requestInput(const std::string& message);

    chtype getInput();

    int getScreenWidth() { return screenWidth; }

    int getScreenHeight() { return screenHeight; }

    void refresh() { screen->refresh(); }

    Screen getScreen() const {
        return *screen;
    }
};