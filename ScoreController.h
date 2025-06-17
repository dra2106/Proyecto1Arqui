#pragma once
#ifndef SCORE_CONTROLLER_H
#define SCORE_CONTROLLER_H

#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "MaxHeap.h"   
#include "KVPair.h"       // Clave: nombre, Valor: puntaje
#include "DLinkedList.h"  

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

    void loadScores() { // Cargar puntajes desde el archivo al heap
        file.open(fileName, std::ios::in);
        if (!file.is_open())
            throw runtime_error("El archivo no se pudo abrir, intente con otro nombre.");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string scoreStr, name;
            if (getline(ss, scoreStr, ',') && getline(ss, name)) {
                int score = stoi(scoreStr);
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
            file << pair.value() << ", " << pair.key() << "\n";
        }

        file.close();
    } // Guardar el heap al archivo (ordenado)
    
public:
    // Constructor
    ScoreController(const string& fileName, const string& playerName)
        : fileName(fileName), playerName(playerName), playerScore(0) {
        loadGame();
    }

    // Guarda el puntaje del jugador actual al archivo
    void saveGame() {
        highScores.insert(KVPair<string, int>(playerName, playerScore));
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
    DLinkedList<KVPair<int, string>> getScores() {
        DLinkedList<KVPair<int, string>> result;
        MaxHeap<KVPair<string, int>> temp = highScores;

        int line = 1;
        while (!temp.isEmpty()) {
            auto pair = temp.removeFirst();
            std::ostringstream oss;
            oss << pair.value() << ", " << pair.key();
            result.append(KVPair<int, string>(line, oss.str()));
            line++;
        }
        return result;
    }

    // Setters
    void setPlayerName(const string& name) { playerName = name; }
    void setPlayerScore(int score) { playerScore = score; }
};

#endif // SCORE_CONTROLLER_H
