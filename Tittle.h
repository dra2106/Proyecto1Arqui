#pragma once

#include <ncurses.h>
#include <string>

class GameUI {
private:
    WINDOW* win;
    int score;

public:
    GameUI() {
        score = 0;
        win = stdscr;
    }

    void drawTitle() {
        std::string title = "=== P H O E N I X ===";
        mvwprintw(win, 0, (COLS - title.size()) / 2, "%s", title.c_str());
    }

    void updateScore(int newScore) {
        score = newScore;
        std::string label = "Score: " + std::to_string(score);
        mvwprintw(win, 0, COLS - label.size() - 1, "%s", label.c_str());
    }

    void refreshUI() {
        wrefresh(win);
    }
};
