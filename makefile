all: Proyecto1Arqui

Proyecto1Arqui: Proyecto1Arqui.cpp Screen.h MainLoop.h Entity.h Spaceship.h Bullet.h SmallBird.h
	g++ Proyecto1Arqui.cpp -lncurses -o Proyecto1Arqui