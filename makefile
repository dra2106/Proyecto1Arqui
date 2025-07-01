all: ProyectoPhoenix

Collision_arm.o: Collision_arm.s
	as -o Collision_arm.o Collision_arm.s

ProyectoPhoenix: ProyectoPhoenix.cpp Collision_arm.o Screen.h Entity.h Spaceship.h Bullet.h SmallBird.h Enemy.h EnemyPatterns.h Mothership.h EnemyController.h MutantBird.h GameAdmin.h NcursesAdmin.h CollisionController.h Profiler.h
	g++ -std=c++17 ProyectoPhoenix.cpp Collision_arm.o -lncurses -o ProyectoPhoenix

clean:
	rm -f ProyectoPhoenix Collision_arm.o

.PHONY: all clean