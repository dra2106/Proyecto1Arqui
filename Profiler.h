#pragma once

#include <chrono>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ncurses.h>

class Profiler {
private:
    using clock = std::chrono::high_resolution_clock;
    clock::time_point startTime;

    float currentFrameMs = 0;
    float currentFPS = 0;

    float minFPS = 9999;
    float maxFPS = 0;
    float totalFPS = 0;

    float minFrame = 9999;
    float maxFrame = 0;
    float totalFrameTime = 0;

    int frameCount = 0;

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
    }

    void saveToFile(const std::string& filename, const std::string& label = "Versión desconocida") const {
        std::ofstream file(filename, std::ios::app); // append para múltiples versiones
        if (!file.is_open()) return;

        float avgFPS = (frameCount > 0) ? totalFPS / frameCount : 0;
        float avgFrame = (frameCount > 0) ? totalFrameTime / frameCount : 0;

        file << "----- AFTERBURNER PAQUETE: " << label << " -----\n";
        file << "Frames procesados: " << frameCount << "\n";
        file << std::fixed << std::setprecision(2);
        file << "FPS -> Avg: " << avgFPS << ", Min: " << minFPS << ", Max: " << maxFPS << "\n";
        file << "Tiempo Frame (ms) -> Avg: " << avgFrame << ", Min: " << minFrame << ", Max: " << maxFrame << "\n";
        file << "--------------------------------------\n\n";

        file.close();
    }

    void drawSummary(WINDOW* win, int y = 100, int x = 1) const {
        float avgFPS = (frameCount > 0) ? totalFPS / frameCount : 0;
        float avgFrame = (frameCount > 0) ? totalFrameTime / frameCount : 0;

        mvwprintw(win, y++, x, "Frames: %d", frameCount);
        mvwprintw(win, y++, x, "FPS Avg: %.2f | Min: %.2f | Max: %.2f", avgFPS, minFPS, maxFPS);
        mvwprintw(win, y++, x, "Frame ms Avg: %.2f | Min: %.2f | Max: %.2f", avgFrame, minFrame, maxFrame);
    }
};
