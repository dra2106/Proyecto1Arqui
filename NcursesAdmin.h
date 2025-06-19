#pragma once
#include <string>
#include <vector>
#include <ncurses.h>

#include "DataStructures/DLinkedList.h"
#include "DataStructures/KVPair.h"

enum class ScreenType {
    START,
    NAME_REQUEST,
    GAME,
    HIGHSCORES,
    GAME_OVER,
    CONTINUE_REQUEST
};

class NcursesAdmin {


private:
    int screenWidth;
    int screenHeight;
    Screen screen;

public:
    NcursesAdmin() {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        getmaxyx(stdscr, screenHeight, screenWidth);
        screen = Screen(screenHeight, screenWidth);
    }
    ~NcursesAdmin() {
        endwin();
    }

    void initializeScreen(){
        screen.initialize();
        curs_set(0); // Hide the cursor
    }

    void finalizeScreen(){
        endwin(); // End ncurses mode
    }

    void clearScreen() {
        screen.clear();
        screen.refresh();
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
                    const std::vector<std::string>& highscores = {});

    void showScreen(ScreenType type, const DLinkedList<KVPair<std::string, int>>& highscores);

    std::string requestInput(const std::string& message);

    chtype getInput();

    Screen getScreen() {
        return screen;
    }

    int getScreenWidth() {
        return screenWidth;
    }

    int getScreenHeight() {
        return screenHeight;
    }

    void refresh() {
        screen.refresh();
    }
};