#pragma once

#include <list>
#include <string>

using std::list;
using std::string;

class ScoreController {
private:
    string currentName;
    int currentScore;
    list<string> scores;

public:
    ScoreController(string name, int score) {
        currentName = name;
        currentScore = score;
    }

    list<string>* getScores() {
        return &scores;
    }

    list<string>* orderScores() {
        return;
    }

    void setCurrentScore(int score) {
        currentScore = score;
    }

    void printScore() {

    }
};