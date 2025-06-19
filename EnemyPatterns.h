// los patrones son vectores de pares, y se almacenan en otro vector
// se busca asignar un patrón al azar dentro del vector grande

#pragma once

#include <vector>

using std::pair;
using std::vector;

namespace EnemyPatterns {
    const vector<vector<pair<int, int>>> SMALL_BIRD_PATTERNS = {
        // Patrón en zigzag
        {{1,0}, {1,0}, {-1,0}, {-1,0}, {0,1}},
        
        // Patrón circular
        {{1,0}, {1,0}, {0,1}, {0,1}, {-1,0}, {-1,0}, {0,-1}, {0,-1}},
        
        // Patrón de caída
        {{0,1}, {0,1}, {1,0}, {1, 0}, {0,1}, {-1,0}, {-1,0}},
        
        // Patrón de avance rápido
        {{2,0}, {2,0}, {-1,1}, {-1,1}}
    };
}