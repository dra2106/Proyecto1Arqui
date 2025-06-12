#pragma once

#include <list>
#include "Enemy.h"

class EnemyController {
public:
	List<Enemy> smallBirds;
	List<Enemy> mediumBirds;
	List<Enemy> mutantBirds;
	Enemy mothershipBirds;
};

