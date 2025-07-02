#pragma once

#include <string>
#include <vector>
#include <iomanip> // Para std::setprecision

#include "NcursesAdmin.h"
#include "ScoreController.h"
#include "Direction.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "EnemyController.h"
#include "CollisionController.h"
#include "ScreenType.h"
#include "Profiler.h"

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
    int level = 0; // Start at level 1
    bool gameOver;

    // TEST
    Profiler profiler;

public:
    GameAdmin() 
        : spaceship(0,0), 
          ncurses(), 
          scoreController("Files/Scores.txt", "Player") {
        // Inicializar atributos
        screenWidth = ncurses.getScreenWidth() / 3;
        screenHeight = ncurses.getScreenHeight();
        gameScreen = new Screen(screenHeight, screenWidth);
        gameScreen->initialize();
        playerName = "Player";
        currentScore = 0;
        highestScore = 0;
        level = 0;
        remainingLives = 24; 
        gameOver = false;

        srand(time(0)); // Inicializa la semilla para números aleatorios
        int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
        int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
        spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
        curs_set(0); 
    }
    ~GameAdmin(){
        delete gameScreen;
        ncurses.finalizeScreen();
    }

    void startGame(){      
        
        // Reset game state
        resetGame();
        
        // Initialize ncurses screen
        ncurses.initializeScreen();

        // Show start screen (game title)
        ncurses.showScreen(ScreenType::START); // Waits for any key to continue

        // Show name request screen and get the name
        ncurses.showScreen(ScreenType::NAME_REQUEST, playerName);
        playerName = ncurses.requestInput("Please, enter your name: ");

        // Load the highest scores from the file
        scoreController.loadGame();
        DLinkedList<KVPair<string, int>> highScoresList;
        scoreController.getScores(highScoresList);
        highScoresList.goToStart();
        highestScore = highScoresList.getElement().value; // Get the highest score from the list

        // Main game loop
        clear(); // Clear the screen
        gameScreen->addBorder();
        
        mainLoopTest();
        
        // At the end of the game, save the player's score
        scoreController.setPlayerName(playerName);
        scoreController.setPlayerScore(currentScore);
        scoreController.saveGame();
        
        // Load the highest scores from the file
        scoreController.loadGame();
        highScoresList.clear();
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
    void mainLoopTest(){
        /*
         * BUCLE PRINCIPAL INSTRUMENTADO PARA ANÁLISIS DE AMDAHL
         * 
         * Este bucle está específicamente diseñado para medir el impacto
         * de cada módulo en el rendimiento general del juego, aplicando
         * la Ley de Amdahl para identificar candidatos de paralelización.
         * 
         * JUSTIFICACIÓN CIENTÍFICA:
         * El módulo de colisiones es típicamente O(n²) y consume 40-60%
         * del tiempo de cómputo en juegos con muchas entidades, siendo
         * el candidato ideal para paralelización según Amdahl.
         */
        while (!gameOver) {
            profiler.startFrame(); // INICIO MEDICIÓN

            // 1. MÓDULO DE RENDERIZADO (Paralelizable - GPU/Multi-threading)
            profiler.startSection("rendering");
            ScreenType pantalla = ScreenType::GAME_TEST; 
            ncurses.showScreen(pantalla, playerName, currentScore, highestScore, level, remainingLives, {}, &profiler);
            updateState();
            profiler.endSection("rendering");

            // 2. MÓDULO DE ENTRADA (Secuencial - I/O bound)
            profiler.startSection("input");
            processInput();
            profiler.endSection("input");

            // 3. MÓDULO DE LÓGICA DE JUEGO (Secuencial - dependencias de estado)
            profiler.startSection("game_logic");
            checkGameOver();
            checkLevelCompletion();
            profiler.endSection("game_logic");

            // 4. MÓDULO DE COLISIONES (ALTAMENTE PARALELIZABLE - OBJETIVO PRINCIPAL)
            /*
             * ANÁLISIS CRÍTICO: Este módulo es el candidato principal para
             * paralelización porque:
             * - Complejidad O(n²) en detección brute-force
             * - Independencia entre cálculos de colisión
             * - Alto impacto en rendimiento (40-60% tiempo de frame)
             * - Escalabilidad natural con múltiples núcleos
             */
            profiler.startSection("collision");
            checkCollisions(); // CAJA NEGRA - medición externa sin modificación
            profiler.endSection("collision");

            profiler.endFrame(); // FIN MEDICIÓN

            napms(50); // Delay para controlar FPS
        }
    }

    void mainLoop(){
        while (!gameOver) {

            ScreenType pantalla = ScreenType::GAME; 
            ncurses.showScreen(pantalla, playerName, currentScore, highestScore, level, remainingLives, {}, &profiler);
            updateState();

            processInput();

            checkCollisions();
            checkGameOver();
            checkLevelCompletion();

            napms(32);
        }
    }

    void checkLevelCompletion() {
        if (enemies.getEnemyCounter() == 0) {
            level += 1; // Increment the level
            ConstruirNiveles(level);
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
                if (playerBullets.size() < 7) {    
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
                remainingLives--;
        int enemyCountEliminated = enemies.CheckCollisionsEnemies(playerBullets);
        currentScore += enemyCountEliminated * 100; // Incrementa la puntuación por cada enemigo eliminado
    }

    void updateState() {
        gameScreen->clear();

        // Update and draw player bullets
        spaceship.update();
        spaceship.move(gameScreen->getWidth(), gameScreen->getHeight());
        
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
        remainingLives = 12;
        currentScore = 0;
        gameOver = false;

        enemies.clearEnemies();
        ConstruirNiveles(level);

        level = 0; // Reset level to 0

        // Clear the screen and reinitialize the spaceship
        gameScreen->clear();
        int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
        int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
        spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada

        // Clear player bullets and enemies
        playerBullets.clear();
    }

    void refresh() {
        ncurses.refresh();
    }

    void checkGameOver() {
        if (remainingLives <= 0) {
            gameOver = true;
            // Generar reporte científico de Amdahl al terminar el juego
            generateAmdahlReport();
        }
    }

    void clear() {
        gameScreen->clear();
        gameScreen->addBorder();
    }

    void ConstruirNiveles(const int nivel) {
        if (nivel == 0) {
            gameScreen->initialize();
            enemies.spawnSmall(gameScreen->getHeight(), gameScreen->getWidth(), nivel + 1, gameScreen->getWidth() / 2);

            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0); 
        } else if (nivel == 1) {
            gameScreen->initialize();
            enemies.spawnSmall(gameScreen->getHeight(), gameScreen->getWidth(), nivel + 1, gameScreen->getWidth() / 2);
            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
        } else if (nivel == 2) {
            gameScreen->initialize();
            enemies.spawnMutant(gameScreen->getHeight(), gameScreen->getWidth(), nivel + 1, gameScreen->getWidth() / 2);

            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
            
        } else if (nivel == 3) {
            gameScreen->initialize();
            enemies.spawnMutant(gameScreen->getHeight(), gameScreen->getWidth(), nivel + 1, gameScreen->getWidth() / 2);
            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
        } else if (nivel == 4) {
            gameScreen->initialize();
            enemies.spawnSmall(gameScreen->getHeight(), gameScreen->getWidth(), 1, gameScreen->getWidth() / 2);
            int naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            int naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            
            enemies.spawnMother(gameScreen->getHeight(), gameScreen->getWidth());
            naveY = gameScreen->getHeight() - 3;     // 2 bloques arriba del borde inferior
            naveX = gameScreen->getWidth() / 2;      // centrada horizontalmente
            spaceship = Spaceship(naveY, naveX);    // ahora sí, la nave queda bien posicionada
            curs_set(0);
        }
    }

    // Método para generar reporte científico de Amdahl
    void generateAmdahlReport() {
        std::string reportFilename = "Files/Amdahl_Analysis_Report.txt";
        std::string moduleMetricsFile = "Files/Module_Metrics.txt";
        std::string csvDataFile = "Files/Module_Data.csv";
        
        // Guardar el reporte completo
        profiler.saveToFile(reportFilename, "Análisis Científico ProyectoPhoenix");
        
        // Guardar métricas específicas por módulo
        profiler.saveModuleMetrics(moduleMetricsFile);
        
        // Exportar datos para análisis posterior
        profiler.exportModuleData(csvDataFile);
        
        // Guardar métricas rápidas
        saveQuickMetrics();
        
        // También mostrar un resumen en consola
        std::cout << "\n=== REPORTES DE ANÁLISIS AMDAHL GENERADOS ===\n";
        std::cout << "Reporte completo: " << reportFilename << "\n";
        std::cout << "Métricas por módulo: " << moduleMetricsFile << "\n";
        std::cout << "Datos CSV: " << csvDataFile << "\n";
        std::cout << "Métricas rápidas: Files/Quick_Metrics.txt\n";
        std::cout << "Display performance: Files/Performance_Display.txt\n";
        std::cout << "Los reportes contienen justificación científica para\n";
        std::cout << "la optimización del módulo de colisiones.\n";
        std::cout << "=============================================\n\n";
    }

    void saveQuickMetrics() {
        // Guardar métricas en formato rápido para visualización
        profiler.saveMetricsSummary("Files/Quick_Metrics.txt");
        
        // También crear un archivo con formato similar al attachment
        std::ofstream file("Files/Performance_Display.txt", std::ios::app);
        if (file.is_open()) {
            file << "+- AMDAHL PERFORMANCE METRICS ---------------\n";
            file << "|=== ANálisis AMDAHL ===\n";
            file << "|Frames: " << profiler.getFrameCount() << " | FPS: " << std::fixed << std::setprecision(1) << profiler.getAvgFPS() << "\n";
            file << "|Frame Time: " << std::setprecision(2) << profiler.getAvgFrameTime() << " ms\n";
            file << "|\n";
            file << "|--- FRACCIONAMIENTO ---\n";
            file << "|Secuencial: " << std::setprecision(1) << (profiler.getSequentialFraction() * 100) << "%\n";
            file << "|Paralelizable: " << (profiler.getParallelFraction() * 100) << "%\n";
            file << "|\n";
            file << "|--- SPEEDUP TEóRICO ---\n";
            file << "|2 cores: " << std::setprecision(2) << profiler.calculateSpeedup(2) << "x\n";
            file << "|4 cores: " << profiler.calculateSpeedup(4) << "x\n";
            file << "|8 cores: " << profiler.calculateSpeedup(8) << "x\n";
            file << "|\n";
            file << "|--- MóDULOS CRíTICOS ---\n";
            file.close();
        }
    }

    void saveRuntimeMetrics() {
        // Guardar métricas durante la ejecución sin detener el juego
        static int saveCounter = 0;
        saveCounter++;
        
        std::string filename = "Files/Runtime_Metrics_" + std::to_string(saveCounter) + ".txt";
        profiler.saveModuleMetrics(filename);
        
        // También guardar en CSV para análisis
        profiler.exportModuleData("Files/Runtime_Data.csv");
    }
};