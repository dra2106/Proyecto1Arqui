

#pragma once

#include <string>
#include <vector>

#include "NcursesAdmin.h"
#include "ScoreController.h"
#include "Direction.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "EnemyController.h"
#include "CollisionController.h"
#include "ScreenType.h"

using std::string;

class GameAdmin {
private:
    // Attributes

    // Game Screen
    int screenWidth;
    int screenHeight;
    bool gameWindowInitialized = false; // Flag to check if the game window is initialized
    Screen* gameScreen; // Pointer to the game screen

    // Game entities
    Spaceship spaceship;
    EnemyController enemies;
    vector<Bullet> playerBullets;

    // Controllers
    NcursesAdmin ncurses;
    ScoreController scoreController;
    CollisionController collisionController;

    // Game state
    string playerName;
    int remainingLives;
    int currentScore;
    int highestScore;
    int level;
    bool gameOver;

public:
    GameAdmin() 
        : spaceship(0,0), 
          ncurses(), 
          scoreController("Files/Scores.txt", "Player") {
        // Initialize game attributes
        playerName = "Player";
        currentScore = 0;
        highestScore = 0;
        level = 1;
        remainingLives = 3;
        gameOver = false;
    }
    ~GameAdmin(){
        // Finalize ncurses screen
        ncurses.finalizeScreen();
    }

    void startGame(){
        // Initialize ncurses screen
        ncurses.initializeScreen();

        // Show start screen (game title)
        ncurses.showScreen(ScreenType::START); // Waits for any key to continue

        // Show name request screen and get the name
        ncurses.showScreen(ScreenType::NAME_REQUEST, playerName);
        playerName = ncurses.requestInput("Please, enter your name: ");

        // Set the player's name in the score controller
        highestScore = scoreController.getHighestScore().value;

        // Main game loop
        ncurses.showScreen(ScreenType::GAME, playerName, currentScore, highestScore, level, 
            remainingLives);
        mainLoop();
        
        // At the end of the game, save the player's score
        scoreController.setPlayerName(playerName);
        scoreController.setPlayerScore(currentScore);
        scoreController.saveGame();
        
        // Load the highest scores from the file
        scoreController.loadGame();
        DLinkedList<KVPair<string, int>> highScoresList;
        scoreController.getScores(highScoresList);
        
        // At the end of the game, show the Game Over screen
        ncurses.showScreen(ScreenType::GAME_OVER, playerName, currentScore, highestScore, level, 
            remainingLives);

        // Show the highest scores
        vector<string> highScoresVector;
        for (highScoresList.goToStart(); !highScoresList.atEnd(); highScoresList.next()) {
            KVPair<string, int> pair = highScoresList.getElement();
            highScoresVector.push_back(pair.key + ": " + std::to_string(pair.value));
        }
        ncurses.showScreen(ScreenType::HIGHSCORES, 
                           playerName, 
                           currentScore,  
                           highestScore, 
                           level, 
                           remainingLives, 
                           highScoresVector);
        
    }

    bool wantsToContinue() {
        ncurses.clearScreen();
        ncurses.showScreen(ScreenType::CONTINUE_REQUEST, playerName, currentScore, highestScore, level, 
            remainingLives);
        char answer = ncurses.requestInput("Do you want to continue playing? (y/n): ")[0];
        
        if (answer == 'y' || answer == 'Y')
            return true; // Continue the game
        else
            return false; // Exit the game
    }

private:
    void mainLoop(){
        while (!gameOver) {
            // Process user input
            processInput();

            // Update game state
            updateState();

            // Refresh the screen
            refresh();

            // Check for collisions
            checkCollisions();

            // Check if the game is over
            checkGameOver();
        }
    }

    void processInput() {
        
        chtype input = ncurses.getInput();

        switch (input) {
            case KEY_UP:            
                //spaceship.activateShield();    
                spaceship.setDirection(STAND);    
                break;
            case KEY_LEFT:
                spaceship.setDirection(LEFT);
                break;
            case KEY_RIGHT:
                spaceship.setDirection(RIGHT);
                break;
            // Al presionar Q
            case 'q':
            case 'Q':
                gameOver = true; // Termina el juego
                break;
            case ' ':
                spaceship.setDirection(STAND);    
                if (playerBullets.size() < 5) {    
                    playerBullets.emplace_back(spaceship.getY() - 1, spaceship.getX());
                    for (Bullet& b : playerBullets) {
                        b.setDirection(UP);   
                    }
                }
                break;
            default:
                spaceship.setDirection(STAND);
                break;
        }
    }

    void checkCollisions() {

        // Check for collisions between player bullets and enemies
        // PENDIENTE

        // Check for collisions between player and enemies their bullets
        if (enemies.checkPlayerCollisions(spaceship)) {
            remainingLives--;
            if (remainingLives <= 0)
                gameOver = true;
        }
    }

    void updateState() {
        // Limpia la pantalla
        //ncurses.clearScreen();

        // Mueve la nave y actualiza su posición
        spaceship.move(ncurses.getScreenWidth(), ncurses.getScreenHeight());

        // Check for collisions between player and enemies
        checkCollisions();

        // Update enemies
        // enemies.updateAll(ncurses.getScreen());
        ncurses.refresh();
    }

    void refresh() {
        ncurses.refresh();
    }

    void checkGameOver() {
        if (remainingLives <= 0)
            gameOver = true;
    }
};