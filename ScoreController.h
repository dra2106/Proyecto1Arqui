/* 
 * Nombre de Archivo: ScoreController.h
 *
 * Descripcion:
 * Este archivo define la clase ScoreController, encargada de gestionar la lectura,
 * almacenamiento y actualización de los puntajes (highscores) del juego desde y hacia un archivo.
 * Utiliza estructuras como MaxHeap y DLinkedList para mantener y ordenar los puntajes.
 *
 * Integrantes del Proyecto:
 * David Rojas Arias
 * Allan José Jimenez Rivera
 * Juan Carlos Monsalve Perez
 * Josué Santiago Hidalgo Sandoval
 */

#pragma once

#ifndef SCORE_CONTROLLER_H
#define SCORE_CONTROLLER_H

#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>


#include "DataStructures/KVPair.h"       // Clave: nombre, Valor: puntaje
#include "DataStructures/DLinkedList.h"  
#include "DataStructures/MaxHeap.h"   

using std::fstream;
using std::string;
using std::runtime_error;

class ScoreController {
private:
    fstream file;
    string fileName;
    string playerName;
    int playerScore;
    MaxHeap<KVPair<string, int>> highScores;

    static std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c){ return std::toupper(c); });
        return result;
    }

    void loadScores() { // Cargar puntajes desde el archivo al heap
        highScores.clear();
        file.open(fileName, std::ios::in);
        if (!file.is_open())
            throw runtime_error("El archivo no se pudo abrir, intente con otro nombre.");
        string line;
        while (getline(file, line)) {
            std::stringstream ss(line);
            string scoreStr, name;
            if (getline(ss, scoreStr, ',') && getline(ss, name)) {
                int score = stoi(scoreStr);
                name = trim(name);
                name = toUpper(name); // <-- Convierte a mayúsculas
                highScores.insert(KVPair<string, int>(name, score));
            }
        }
        file.close();
    } 

    void saveScores() {
        DLinkedList<KVPair<string, int>> sortedScores;
        MaxHeap<KVPair<string, int>> temp = highScores;

        int count = 0;
        while (!temp.isEmpty() && count < 10) {
            sortedScores.append(temp.removeFirst());
            count++;
        }

        file.open(fileName, std::ios::out | std::ios::trunc);
        
        for (sortedScores.goToStart(); !sortedScores.atEnd(); sortedScores.next()) {
            KVPair<string, int> pair = sortedScores.getElement();
            file << pair.value << ", " << toUpper(trim(pair.key)) << "\n"; // <-- Convierte a mayúsculas
        }

        file.close();
    }
    
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

public:
    // Constructor
    ScoreController(const string& fileName, const string& playerName)
        : fileName(fileName), playerName(playerName), playerScore(0) {
        loadGame();
    }

    // Guarda el puntaje del jugador actual al archivo
    void saveGame() {
        // Copia temporal para buscar nombres existentes
        MaxHeap<KVPair<string, int>> temp = highScores;
        string uniqueName = playerName;
        bool exists = true;

        // Busca un nombre único
        while (exists) {
            exists = false;
            MaxHeap<KVPair<string, int>> checkHeap = temp; // Copia para iterar
            while (!checkHeap.isEmpty()) {
                KVPair<string, int> pair = checkHeap.removeFirst();
                if (pair.key == uniqueName) {
                    uniqueName += "A";
                    exists = true;
                    break;
                }
            }
        }

        highScores.insert(KVPair<string, int>(uniqueName, playerScore));
        saveScores();
    }

    // Carga de memoria el puntaje del jugador actual al archivo
    void loadGame(){
        try { loadScores(); }
        catch (const runtime_error& e) {    
            cout << "ERROR: Runtime Error." << endl;
            cout << "Detalle: " << e.what() << endl;
	    }   
    }

    // Devuelve los puntajes como lista de pares (línea, texto)
    void getScores(DLinkedList<KVPair<string, int>>& result) {
        std::vector<KVPair<string, int>> tempVec;

        MaxHeap<KVPair<string, int>> temp = highScores;
        while (!temp.isEmpty()) {
            tempVec.push_back(temp.removeFirst());
        }

        // Ordena de mayor a menor puntaje
        std::sort(tempVec.begin(), tempVec.end(), [](const KVPair<string, int>& a, const KVPair<string, int>& b) {
            return a.value > b.value;
        });

        result.clear();
        for (const auto& pair : tempVec) {
            result.append(pair);
        }
    }

    // Setters
    void setPlayerName(const string& name) { playerName = name; }
    void setPlayerScore(int score) { playerScore = score; }

    // Getters
    string getPlayerName() const { return playerName; }
    int getPlayerScore() const { return playerScore; }

    KVPair<string, int> getHighestScore() {
        return highScores.first();
    }
    
};

#endif // SCORE_CONTROLLER_H
