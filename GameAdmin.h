

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
        screenWidth = ncurses.getScreenWidth() / 3;
        screenHeight = ncurses.getScreenHeight();
        gameScreen = new Screen(screenHeight, screenWidth); // Allocate Screen
        gameScreen->initialize();
        playerName = "Player";
        currentScore = 0;
        highestScore = 0;
        level = 1;
        remainingLives = 9;
        gameOver = false;

        srand(time(0)); // Inicializa la semilla para números aleatorios
        int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
        int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
        spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
        curs_set(0); 

        ConstruirNiveles(1); // Initialize levels
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
        clear(); // Clear the screen
        gameScreen->addBorder();
        
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
        
        if (answer == 'y' || answer == 'Y'){
            resetGame();
            return true; // Continue the game
        }
        else
            return false; // Exit the game
    }

private:
    void mainLoop(){
        while (!gameOver) {
            // Show the game screen with current state
            ncurses.showScreen(ScreenType::GAME, playerName, currentScore, highestScore, level, 
            remainingLives);

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

            napms(32); // Sleep for a short duration to control game speed
        }
    }

    void processInput() {

        chtype input = gameScreen->getInput();

        switch (input) {
            case KEY_UP:            
                spaceship.activateShield();
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
        if (!spaceship.isShieldActive())
            if (enemies.checkPlayerCollisions(spaceship))
                remainingLives--; // si hay colisión, se resta una vida
        int enemyCountEliminated = enemies.CheckCollisionsEnemies(playerBullets);
        currentScore += enemyCountEliminated * 100; // Incrementa la puntuación por cada enemigo eliminado
    }

    void updateState() {
        gameScreen->clear();

        // Update and draw player bullets
        spaceship.update();
        spaceship.move(gameScreen->getWidth(), gameScreen->getHeight());

        // If the spaceship eliminates all enemies, build the next level
        if (enemies.getEnemyCounter() == 0)
            ConstruirNiveles(++level);
        
        enemies.updateAll(*gameScreen);

        // Show the spaceship and its bullets
        for (Bullet& b : playerBullets) {
            b.move();
            gameScreen->add(b);
        }

        playerBullets.erase(
            std::remove_if(playerBullets.begin(), playerBullets.end(),
                [](const Bullet& b) { return b.getY() < 1; }),
            playerBullets.end()
        );
        
        gameScreen->add(spaceship);
    }

    void resetGame() {
        // Reset game state
        remainingLives = 3;
        currentScore = 0;
        level = 1;
        gameOver = false;

        // Clear the screen and reinitialize the spaceship
        gameScreen->clear();
        int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
        int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
        spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada

        // Clear player bullets and enemies
        playerBullets.clear();
        // enemies.clearEnemies(); // PENDIENTE
    }

    void refresh() {
        ncurses.refresh();
    }

    void checkGameOver() {
        if (remainingLives <= 0)
            gameOver = true;
    }

    void clear() {
        gameScreen->clear();
        gameScreen->addBorder();
    }

    void ConstruirNiveles(int nivel) {
        if (nivel == 1) {
            gameScreen->initialize();
            enemies.spawnSmall(gameScreen->getHeight(), gameScreen->getWidth(), 3, gameScreen->getWidth() / 2);

            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0); 
        } else if (nivel == 2) {
            gameScreen->initialize();
            enemies.spawnSmall(gameScreen->getHeight(), gameScreen->getWidth(), nivel, gameScreen->getWidth() / 2);
            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
        } else if (nivel == 3) {
            gameScreen->initialize();
            enemies.spawnMutant(gameScreen->getHeight(), gameScreen->getWidth(), nivel, gameScreen->getWidth() / 2);

            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
            
        } else if (nivel == 4) {
            gameScreen->initialize();
            enemies.spawnMutant(gameScreen->getHeight(), gameScreen->getWidth(), nivel, gameScreen->getWidth() / 2);
            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
        } else if (nivel == 5) {
        }
    }
};