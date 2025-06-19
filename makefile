all: Proyecto1Arqui

Proyecto1Arqui: Proyecto1Arqui.cpp Screen.h MainLoop.h Entity.h Spaceship.h Bullet.h SmallBird.h Enemy.h EnemyPatterns.h Mothership.h EnemyController.h MutantBird.h Tittle.h
	g++ Proyecto1Arqui.cpp -lncurses -o Proyecto1Arqui