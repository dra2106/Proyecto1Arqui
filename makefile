# Nombre de Archivo: makefile
#
# Descripcion:
# Este archivo define las reglas de compilación para el proyecto.
# Especifica cómo compilar y enlazar los archivos fuente y cabecera
# para generar el ejecutable principal del juego utilizando g++ y la biblioteca ncurses.
#
# Integrantes del Proyecto:
# David Rojas Arias
# Allan José Jimenez Rivera
# Juan Carlos Monsalve Perez
# Josué Santiago Hidalgo Sandoval

all: ProyectoPhoenix

Collision_arm.o: Collision_arm.s
	as -o Collision_arm.o Collision_arm.s

ProyectoPhoenix: ProyectoPhoenix.cpp Collision_arm.o Screen.h Entity.h Spaceship.h Bullet.h SmallBird.h Enemy.h EnemyPatterns.h Mothership.h EnemyController.h MutantBird.h GameAdmin.h NcursesAdmin.h CollisionController.h
	g++ ProyectoPhoenix.cpp Collision_arm.o -lncurses -o ProyectoPhoenix

clean:
	rm -f *.o ProyectoPhoenix