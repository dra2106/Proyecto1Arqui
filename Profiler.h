#pragma once

#include <chrono>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ncurses.h>
#include <map>
#include <ctime>
#include <algorithm>
#include <vector>

class Profiler {
private:
    using clock = std::chrono::steady_clock;
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

    // Agregar más categorías específicas para identificar cuellos de botella
    void endSection(const std::string& sectionName) {
        if (sectionStartTimes.find(sectionName) != sectionStartTimes.end()) {
            auto endTime = clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                endTime - sectionStartTimes[sectionName]).count();
            float timeMs = duration / 1000.0f;
            
            sectionTotalTimes[sectionName] += timeMs;
            sectionCounts[sectionName]++;
            
            // Categorización mejorada para análisis de Amdahl
            if (sectionName == "rendering" || sectionName == "graphics" || sectionName == "draw") {
                // Renderizado: Potencialmente paralelizable (GPU/multi-threading)
                totalParallelTime += timeMs;
            } else if (sectionName == "input" || sectionName == "file_io" || sectionName == "ncurses_refresh") {
                // I/O: Generalmente secuencial por naturaleza
                totalIOTime += timeMs;
                totalSequentialTime += timeMs;
            } else if (sectionName == "collision") {
                // MÓDULO CRÍTICO: Colisiones - altamente paralelizable
                // Este es el candidato principal para optimización según Amdahl
                totalParallelTime += timeMs;
            } else if (sectionName == "game_logic" || sectionName == "state_update") {
                // Lógica de juego: Generalmente secuencial
                totalLogicTime += timeMs;
                totalSequentialTime += timeMs;
            } else {
                // Por defecto, asumir secuencial para ser conservador
                totalSequentialTime += timeMs;
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

        file << "===============================================\n";
        file << "    REPORTE CIENTÍFICO DE ANÁLISIS AMDAHL    \n";
        file << "===============================================\n";
        file << "Proyecto: ProyectoPhoenix - Juego C++ ncurses\n";
        file << "Análisis: " << label << "\n";
        file << "Fecha: " << getCurrentTimestamp() << "\n\n";

        file << "1. MÉTRICAS GENERALES DE RENDIMIENTO\n";
        file << "------------------------------------\n";
        file << "Frames procesados: " << frameCount << "\n";
        file << std::fixed << std::setprecision(3);
        file << "FPS promedio: " << avgFPS << " (Min: " << minFPS << ", Max: " << maxFPS << ")\n";
        file << "Tiempo promedio por frame: " << avgFrame << " ms\n\n";
        
        // Análisis detallado por módulos
        file << "2. ANÁLISIS POR MÓDULOS (LEY DE AMDAHL)\n";
        file << "--------------------------------------\n";
        float totalExecutionTime = totalSequentialTime + totalParallelTime;
        
        file << "Tiempo total de ejecución medido: " << totalExecutionTime << " ms\n";
        file << "Fracción secuencial: " << (getSequentialFraction() * 100) << "%\n";
        file << "Fracción paralelizable: " << (getParallelFraction() * 100) << "%\n\n";
        
        file << "TIEMPOS POR MÓDULO (análisis detallado):\n";
        for (const auto& section : sectionTotalTimes) {
            float avgTime = (sectionCounts.at(section.first) > 0) ? 
                           section.second / sectionCounts.at(section.first) : 0;
            float percentage = (totalExecutionTime > 0) ? (section.second / totalExecutionTime) * 100 : 0;
            file << "  • " << section.first << ": " << avgTime << " ms/frame (" << percentage << "% del total)\n";
        }
        
        file << "\n3. JUSTIFICACIÓN CIENTÍFICA PARA OPTIMIZACIÓN\n";
        file << "---------------------------------------------\n";
        
        // Encontrar el módulo con mayor impacto
        std::string bottleneckModule = "";
        float maxTime = 0;
        for (const auto& section : sectionTotalTimes) {
            if (section.second > maxTime) {
                maxTime = section.second;
                bottleneckModule = section.first;
            }
        }
        
        file << "MÓDULO CRÍTICO IDENTIFICADO: " << bottleneckModule << "\n";
        file << "Tiempo total consumido: " << maxTime << " ms\n";
        file << "Impacto en rendimiento: " << ((totalExecutionTime > 0) ? (maxTime / totalExecutionTime) * 100 : 0) << "%\n\n";
        
        file << "APLICACIÓN DE LA LEY DE AMDAHL:\n";
        file << "Según Amdahl: Speedup = 1 / (f + (1-f)/n)\n";
        file << "Donde f = fracción secuencial, n = número de procesadores\n\n";
        
        file << "4. PREDICCIONES DE SPEEDUP TEÓRICO\n";
        file << "-----------------------------------\n";
        for (int p = 1; p <= 16; p *= 2) {
            float speedup = calculateSpeedup(p);
            file << "Con " << p << " núcleo(s): " << speedup << "x speedup teórico\n";
        }
        
        file << "\n5. RECOMENDACIONES BASADAS EN EVIDENCIA\n";
        file << "---------------------------------------\n";
        file << "1. PRIORIDAD ALTA: Optimizar el módulo '" << bottleneckModule << "'\n";
        file << "   Razón: Representa el mayor cuello de botella identificado\n";
        file << "   Impacto esperado: Reducción significativa en tiempo de frame\n\n";
        
        if (bottleneckModule == "collision" || bottleneckModule == "game_logic") {
            file << "2. ESTRATEGIA RECOMENDADA: Paralelización del sistema de colisiones\n";
            file << "   - Implementar detección de colisiones en paralelo\n";
            file << "   - Usar técnicas de particionado espacial\n";
            file << "   - Optimizar algoritmos de detección de intersección\n\n";
        }
        
        file << "3. JUSTIFICACIÓN MATEMÁTICA:\n";
        file << "   Si optimizamos el " << ((totalExecutionTime > 0) ? (maxTime / totalExecutionTime) * 100 : 0) << "% más crítico del código,\n";
        file << "   el speedup máximo teórico sería significativo según Amdahl.\n\n";
        
        file << "===============================================\n";
        file << "Fin del reporte científico\n";
        file << "===============================================\n\n";
        file.close();
    }

    void saveModuleMetrics(const std::string& filename = "module_metrics.txt") const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) return;

        float avgFPS = (frameCount > 0) ? totalFPS / frameCount : 0;
        float avgFrame = (frameCount > 0) ? totalFrameTime / frameCount : 0;
        float totalExecutionTime = totalSequentialTime + totalParallelTime;

        file << "+- AMDAHL PERFORMANCE METRICS ---------------\n";
        file << "|=== ANÁLISIS AMDAHL ===\n";
        file << "|Frames: " << frameCount << " | FPS: " << std::fixed << std::setprecision(1) << avgFPS << "\n";
        file << "|Frame Time: " << std::setprecision(2) << avgFrame << " ms\n";
        file << "|\n";
        file << "|--- FRACCIONAMIENTO ---\n";
        file << "|Secuencial: " << std::setprecision(1) << (getSequentialFraction() * 100) << "%\n";
        file << "|Paralelizable: " << (getParallelFraction() * 100) << "%\n";
        file << "|\n";
        file << "|--- SPEEDUP TEÓRICO ---\n";
        file << "|2 cores: " << std::setprecision(2) << calculateSpeedup(2) << "x\n";
        file << "|4 cores: " << calculateSpeedup(4) << "x\n";
        file << "|8 cores: " << calculateSpeedup(8) << "x\n";
        file << "|\n";
        file << "|--- MÓDULOS CRÍTICOS ---\n";
        
        // Encontrar el módulo más crítico
        std::string bottleneck = "";
        float maxTime = 0;
        
        for (const auto& section : sectionTotalTimes) {
            if (section.second > maxTime) {
                maxTime = section.second;
                bottleneck = section.first;
            }
        }
        
        if (!bottleneck.empty()) {
            float impact = (totalExecutionTime > 0) ? (maxTime / totalExecutionTime) * 100 : 0;
            file << "|Crítico: " << bottleneck << "\n";
            file << "|Impacto: " << std::setprecision(1) << impact << "%\n";
        }
        
        file << "|\n";
        file << "|--- TIEMPOS DETALLADOS POR MÓDULO ---\n";
        for (const auto& section : sectionTotalTimes) {
            float avgTime = (sectionCounts.at(section.first) > 0) ? 
                           section.second / sectionCounts.at(section.first) : 0;
            float percentage = (totalExecutionTime > 0) ? (section.second / totalExecutionTime) * 100 : 0;
            file << "|" << section.first << ": " << std::setprecision(3) << avgTime 
                 << " ms (" << std::setprecision(1) << percentage << "%)\n";
        }
        
        file << "|\n";
        file << "|--- MÉTRICAS ESPECÍFICAS ---\n";
        file << "|Tiempo I/O total: " << std::setprecision(3) << totalIOTime << " ms\n";
        file << "|Tiempo lógica total: " << totalLogicTime << " ms\n";
        file << "|Tiempo secuencial total: " << totalSequentialTime << " ms\n";
        file << "|Tiempo paralelizable total: " << totalParallelTime << " ms\n";
        file << "|\n";
        file << "|--- RECOMENDACIÓN ---\n";
        if (!bottleneck.empty()) {
            file << "|Optimizar: " << bottleneck << "\n";
            if (bottleneck == "game_logic" || bottleneck.find("collision") != std::string::npos) {
                file << "|Estrategia: Paralelizar\n";
                file << "|colisiones\n";
            }
        }
        
        file << "+- Timestamp: " << getCurrentTimestamp() << " -------\n\n";
        file.close();
    }

    void saveMetricsSummary(const std::string& filename = "metrics_summary.txt") const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) return;

        float avgFPS = (frameCount > 0) ? totalFPS / frameCount : 0;
        float avgFrame = (frameCount > 0) ? totalFrameTime / frameCount : 0;
        float totalExecutionTime = totalSequentialTime + totalParallelTime;

        // Formato similar al attachment
        file << "+- AMDAHL PERFORMANCE METRICS ---------------\n";
        file << "|=== ANálisis AMDAHL ===\n";
        file << "|Frames: " << frameCount << " | FPS: " << std::fixed << std::setprecision(1) << avgFPS << "\n";
        file << "|Frame Time: " << std::setprecision(2) << avgFrame << " ms\n";
        file << "|\n";
        file << "|--- FRACCIONAMIENTO ---\n";
        file << "|Secuencial: " << std::setprecision(1) << (getSequentialFraction() * 100) << "%\n";
        file << "|Paralelizable: " << (getParallelFraction() * 100) << "%\n";
        file << "|\n";
        file << "|--- SPEEDUP TEÓRICO ---\n";
        file << "|2 cores: " << std::setprecision(2) << calculateSpeedup(2) << "x\n";
        file << "|4 cores: " << calculateSpeedup(4) << "x\n";
        file << "|8 cores: " << calculateSpeedup(8) << "x\n";
        file << "|\n";
        file << "|--- MÓDULOS CRÍTICOS ---\n";
        
        // Análisis detallado por módulos ordenados por impacto
        std::vector<std::pair<std::string, float>> modulesByTime;
        for (const auto& section : sectionTotalTimes) {
            modulesByTime.push_back({section.first, section.second});
        }
        
        // Ordenar por tiempo (mayor a menor)
        std::sort(modulesByTime.begin(), modulesByTime.end(), 
                  [](const auto& a, const auto& b) { return a.second > b.second; });
        
        for (const auto& module : modulesByTime) {
            float avgTime = (sectionCounts.at(module.first) > 0) ? 
                           module.second / sectionCounts.at(module.first) : 0;
            float percentage = (totalExecutionTime > 0) ? (module.second / totalExecutionTime) * 100 : 0;
            file << "|" << module.first << ": " << std::setprecision(3) << avgTime 
                 << " ms/frame (" << std::setprecision(1) << percentage << "%)\n";
        }
        
        file << "+---------------------------------------------\n\n";
        file.close();
    }

    void saveAmdahlMetrics(const std::string& filename = "amdahl_metrics.txt") const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) return;

        float avgFPS = (frameCount > 0) ? totalFPS / frameCount : 0;
        float avgFrame = (frameCount > 0) ? totalFrameTime / frameCount : 0;

        // Formato exacto del attachment
        file << "+- AMDAHL PERFORMANCE METRICS ---------------\n";
        file << "|=== ANálisis AMDAHL ===\n";
        file << "|Frames: " << frameCount << " | FPS: " << std::fixed << std::setprecision(1) << avgFPS << "\n";
        file << "|Frame Time: " << std::setprecision(2) << avgFrame << " ns\n";
        file << "|\n";
        file << "|--- FRACCIONAMIENTO ---\n";
        file << "|Secuencial: " << std::setprecision(1) << (getSequentialFraction() * 100) << "%\n";
        file << "|Paralelizable: " << (getParallelFraction() * 100) << "%\n";
        file << "|\n";
        file << "|--- SPEEDUP TEóRICO ---\n";
        file << "|2 cores: " << std::setprecision(2) << calculateSpeedup(2) << "x\n";
        file << "|4 cores: " << calculateSpeedup(4) << "x\n";
        file << "|8 cores: " << calculateSpeedup(8) << "x\n";
        file << "|\n";
        file << "|--- MóDULOS CRíTICOS ---\n";
        
        file.close();
    }

    void exportModuleData(const std::string& filename = "module_data.csv") const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) return;

        float totalExecutionTime = totalSequentialTime + totalParallelTime;
        
        // Header si es un archivo nuevo
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "Timestamp,Module,AvgTimeMs,TotalTimeMs,Percentage,CallCount\n";
        }
        
        std::string timestamp = getCurrentTimestamp();
        
        for (const auto& section : sectionTotalTimes) {
            float avgTime = (sectionCounts.at(section.first) > 0) ? 
                           section.second / sectionCounts.at(section.first) : 0;
            float percentage = (totalExecutionTime > 0) ? (section.second / totalExecutionTime) * 100 : 0;
            
            file << timestamp << ","
                 << section.first << ","
                 << std::fixed << std::setprecision(4) << avgTime << ","
                 << section.second << ","
                 << std::setprecision(2) << percentage << ","
                 << sectionCounts.at(section.first) << "\n";
        }
        
        file.close();
    }

private:
    std::string getCurrentTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:

    void drawSummary(WINDOW* win, int y = 1, int x = 1) const {
        float avgFPS = (frameCount > 0) ? totalFPS / frameCount : 0;
        float avgFrame = (frameCount > 0) ? totalFrameTime / frameCount : 0;

        // Escribir directamente en la ventana proporcionada
        if (!win) return;
        
        int row = y;
        mvwprintw(win, row++, x, "=== ANÁLISIS AMDAHL ===");
        mvwprintw(win, row++, x, "Frames: %d | FPS: %.1f", frameCount, avgFPS);
        mvwprintw(win, row++, x, "Frame Time: %.2f ms", avgFrame);
        
        row++;
        mvwprintw(win, row++, x, "--- FRACCIONAMIENTO ---");
        mvwprintw(win, row++, x, "Secuencial: %.1f%%", getSequentialFraction() * 100);
        mvwprintw(win, row++, x, "Paralelizable: %.1f%%", getParallelFraction() * 100);
        
        row++;
        mvwprintw(win, row++, x, "--- SPEEDUP TEÓRICO ---");
        mvwprintw(win, row++, x, "2 cores: %.2fx", calculateSpeedup(2));
        mvwprintw(win, row++, x, "4 cores: %.2fx", calculateSpeedup(4));
        mvwprintw(win, row++, x, "8 cores: %.2fx", calculateSpeedup(8));
        
        row++;
        mvwprintw(win, row++, x, "--- MÓDULOS CRÍTICOS ---");
        
        // Encontrar el módulo más crítico
        std::string bottleneck = "";
        float maxTime = 0;
        float totalTime = totalSequentialTime + totalParallelTime;
        
        for (const auto& section : sectionTotalTimes) {
            if (section.second > maxTime) {
                maxTime = section.second;
                bottleneck = section.first;
            }
        }
        
        if (!bottleneck.empty()) {
            float impact = (totalTime > 0) ? (maxTime / totalTime) * 100 : 0;
            mvwprintw(win, row++, x, "Crítico: %s", bottleneck.c_str());
            mvwprintw(win, row++, x, "Impacto: %.1f%%", impact);
        }
        
        row++;
        mvwprintw(win, row++, x, "--- TIEMPOS DETALLADOS ---");
        for (const auto& section : sectionTotalTimes) {
            float avgTime = (sectionCounts.at(section.first) > 0) ? 
                           section.second / sectionCounts.at(section.first) : 0;
            mvwprintw(win, row++, x, "%s: %.2f ms", section.first.c_str(), avgTime);
        }
        
        row++;
        mvwprintw(win, row++, x, "--- RECOMENDACIÓN ---");
        if (!bottleneck.empty()) {
            mvwprintw(win, row++, x, "Optimizar: %s", bottleneck.c_str());
            if (bottleneck == "game_logic" || bottleneck.find("collision") != std::string::npos) {
                mvwprintw(win, row++, x, "Estrategia: Paralelizar");
                mvwprintw(win, row++, x, "colisiones");
            }
        }
    }

    // Getter methods for accessing metrics
    int getFrameCount() const { return frameCount; }
    
    float getAvgFPS() const { 
        return (frameCount > 0) ? totalFPS / frameCount : 0; 
    }
    
    float getAvgFrameTime() const { 
        return (frameCount > 0) ? totalFrameTime / frameCount : 0; 
    }
    
    float getCurrentFPS() const { return currentFPS; }
    
    float getCurrentFrameTime() const { return currentFrameMs; }
};