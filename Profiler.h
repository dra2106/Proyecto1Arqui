#pragma once

#include <chrono>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ncurses.h>
#include <map>

class Profiler {
private:
    using clock = std::chrono::high_resolution_clock;
    clock::time_point startTime;
    
    // Métricas existentes
    float currentFrameMs = 0;
    float currentFPS = 0;
    float minFPS = 9999;
    float maxFPS = 0;
    float totalFPS = 0;
    float minFrame = 9999;
    float maxFrame = 0;
    float totalFrameTime = 0;
    int frameCount = 0;

    // Nuevas métricas para Amdahl
    std::map<std::string, clock::time_point> sectionStartTimes;
    std::map<std::string, float> sectionTotalTimes;
    std::map<std::string, int> sectionCounts;
    
    // Categorías específicas para análisis de Amdahl
    float totalSequentialTime = 0;   // Tiempo secuencial total
    float totalParallelTime = 0;     // Tiempo paralelizable total
    float totalIOTime = 0;           // Tiempo I/O
    float totalLogicTime = 0;        // Tiempo lógica del juego

public:
    void startFrame() {
        startTime = clock::now();
    }

    void endFrame() {
        auto endTime = clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        currentFrameMs = duration / 1000.0f;
        currentFPS = (currentFrameMs > 0) ? 1000.0f / currentFrameMs : 0;

        frameCount++;
        totalFPS += currentFPS;
        totalFrameTime += currentFrameMs;

        if (currentFPS < minFPS) minFPS = currentFPS;
        if (currentFPS > maxFPS) maxFPS = currentFPS;
        if (currentFrameMs < minFrame) minFrame = currentFrameMs;
        if (currentFrameMs > maxFrame) maxFrame = currentFrameMs;
    }

    // Nuevos métodos para medir secciones específicas
    void startSection(const std::string& sectionName) {
        sectionStartTimes[sectionName] = clock::now();
    }

    void endSection(const std::string& sectionName) {
        if (sectionStartTimes.find(sectionName) != sectionStartTimes.end()) {
            auto endTime = clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                endTime - sectionStartTimes[sectionName]).count();
            float timeMs = duration / 1000.0f;
            
            sectionTotalTimes[sectionName] += timeMs;
            sectionCounts[sectionName]++;
            
            // Categorizar para análisis de Amdahl
            if (sectionName == "rendering" || sectionName == "graphics") {
                totalParallelTime += timeMs;
            } else if (sectionName == "input" || sectionName == "file_io") {
                totalIOTime += timeMs;
                totalSequentialTime += timeMs;
            } else if (sectionName == "game_logic" || sectionName == "collision") {
                totalLogicTime += timeMs;
                totalSequentialTime += timeMs;  // Asumimos que es secuencial
            }
        }
    }

    // Calcular métricas para Amdahl
    float getSequentialFraction() const {
        float totalExecutionTime = totalSequentialTime + totalParallelTime;
        return (totalExecutionTime > 0) ? totalSequentialTime / totalExecutionTime : 0;
    }

    float getParallelFraction() const {
        return 1.0f - getSequentialFraction();
    }

    float calculateSpeedup(int numProcessors) const {
        float seqFraction = getSequentialFraction();
        float parallelFraction = getParallelFraction();
        
        // Ley de Amdahl: Speedup = 1 / (seqFraction + parallelFraction/numProcessors)
        return 1.0f / (seqFraction + parallelFraction / numProcessors);
    }

    void resetStats() {
        currentFrameMs = 0;
        currentFPS = 0;
        frameCount = 0;
        totalFPS = 0;
        totalFrameTime = 0;
        minFPS = 9999;
        maxFPS = 0;
        minFrame = 9999;
        maxFrame = 0;
        
        // Reset nuevas métricas
        sectionTotalTimes.clear();
        sectionCounts.clear();
        totalSequentialTime = 0;
        totalParallelTime = 0;
        totalIOTime = 0;
        totalLogicTime = 0;
    }

    void saveToFile(const std::string& filename, const std::string& label = "Versión desconocida") const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) return;

        float avgFPS = (frameCount > 0) ? totalFPS / frameCount : 0;
        float avgFrame = (frameCount > 0) ? totalFrameTime / frameCount : 0;

        file << "----- AMDAHL BENCHMARK: " << label << " -----\n";
        file << "Frames procesados: " << frameCount << "\n";
        file << std::fixed << std::setprecision(3);
        file << "FPS -> Avg: " << avgFPS << ", Min: " << minFPS << ", Max: " << maxFPS << "\n";
        file << "Tiempo Frame (ms) -> Avg: " << avgFrame << ", Min: " << minFrame << ", Max: " << maxFrame << "\n";
        
        // Métricas de Amdahl
        file << "\n--- ANÁLISIS DE AMDAHL ---\n";
        file << "Fracción Secuencial: " << (getSequentialFraction() * 100) << "%\n";
        file << "Fracción Paralelizable: " << (getParallelFraction() * 100) << "%\n";
        file << "Tiempo Secuencial Total: " << totalSequentialTime << " ms\n";
        file << "Tiempo Paralelizable Total: " << totalParallelTime << " ms\n";
        
        file << "\nSpeedup Teórico:\n";
        for (int p = 1; p <= 16; p *= 2) {
            file << "  " << p << " cores: " << calculateSpeedup(p) << "x\n";
        }
        
        file << "\nTiempos por Sección:\n";
        for (const auto& section : sectionTotalTimes) {
            float avgTime = (sectionCounts.at(section.first) > 0) ? 
                           section.second / sectionCounts.at(section.first) : 0;
            file << "  " << section.first << ": " << avgTime << " ms (avg)\n";
        }
        
        file << "--------------------------------------\n\n";
        file.close();
    }

    void drawSummary(WINDOW* win, int y = 1, int x = 1) const {
        float avgFPS = (frameCount > 0) ? totalFPS / frameCount : 0;
        float avgFrame = (frameCount > 0) ? totalFrameTime / frameCount : 0;

        // Escribir directamente en la ventana proporcionada
        if (!win) return;
        
        int row = y;
        mvwprintw(win, row++, x, "Frames: %d", frameCount);
        mvwprintw(win, row++, x, "FPS: %.2f | Min: %.2f | Max: %.2f", avgFPS, minFPS, maxFPS);
        mvwprintw(win, row++, x, "Frame: %.2f ms | Min: %.2f | Max: %.2f", avgFrame, minFrame, maxFrame);
        
        row++;
        mvwprintw(win, row++, x, "Sequential: %.1f%%", getSequentialFraction() * 100);
        mvwprintw(win, row++, x, "Parallel: %.1f%%", getParallelFraction() * 100);
        
        row++;
        mvwprintw(win, row++, x, "Speedup 2 cores: %.2fx", calculateSpeedup(2));
        mvwprintw(win, row++, x, "Speedup 4 cores: %.2fx", calculateSpeedup(4));
        mvwprintw(win, row++, x, "Speedup 8 cores: %.2fx", calculateSpeedup(8));
        
        row++;
        mvwprintw(win, row++, x, "Seq Time: %.2f ms", totalSequentialTime);
        mvwprintw(win, row++, x, "Par Time: %.2f ms", totalParallelTime);
    }
};