#pragma once
#include "Entity.h"

#define SCREEN_COLS 30

enum Direction {
	up,
	left,
	right,
	stand
};

class Spaceship : public Entity {
private:
	Direction currentDirection;
	//int hearts;
	//bool inmune;
	//int cooldown;
	//std::list <std::string> sprite;
public:
	Spaceship() {
		this->y = this->x = 0;
	}
	Spaceship(int y, int x) {
		this->y = y;
		this->x = x;
		this->icon = '^';
		// x_ = 0;
		// y_ = 0;
		// hearts = 3;
		// inmune = false;
		// cooldown = 0;
		// sprite = { "" };
	}
	~Spaceship() {
		//sprite.clear();
	}

	void setDirection(Direction d) {
		currentDirection = d;
	}

	Direction getDirection() {
		return currentDirection;
	}

	void move() {
		switch (currentDirection) {
		case left:
			x--;
			break;
		case right:
			x++;
			break;
		default:
			break;
		}
		// lo siguiente es para evitar que la nave se salga de la pantalla
		if (x < 1) 
			x = 1; 				 // si la posición es menor a 0 (el borde izquierdo), se regresa
		if (x > SCREEN_COLS - 2) 
			x = SCREEN_COLS - 2; // lo mismo pero con el borde derecho
	}

	int getY() {
		return y;
	}

	int getX() {
		return x;
	}
};
	// void moveLeft() {
	// 	x--;

	// }

	// void moveRight() {
	// 	x++;
	// 	// esta parte es para que no se salga de la caja

	// }

	// int getMove() {
	// 	int choice = wgetch(currentWindow);
	// 	switch(choice) {
	// 		case KEY_UP: // por ahora, arriba no hace nada
	// 			break;
	// 		case KEY_DOWN: // abajo no hace nada
	// 			break;
	// 		case KEY_LEFT:
	// 			moveLeft();
	// 			break;
	// 		case KEY_RIGHT:
	// 			moveRight();
	// 			break;
	// 		default:
	// 			break;
	// 	}
	// 	return choice;
	// }

	// void display() {
	// 	mvwaddch(currentWindow, yLoc, xLoc, character);
	// }

	// void setX(int x) {
	// 	x_ = x;
	// }
	// void setY(int y) {
	// 	y_ = y;
	// }
	// int getX(){
	// 	return x_;
	// }
	// int getY() {
	// 	return y_;
	// }
	// void setInmune(bool inm) {
	// 	inmune = inm;
	// }
	// void setCooldown(int cooldown) {
	// 	this->cooldown = cooldown;
	// }
	// void setHearts(int hearts) {
	// 	this->hearts = hearts;
	// }
	// bool isInmune() {
	// 	return inmune;
	// }
	// int getCooldown(){
	// 	return cooldown;
	// }
	// int getHearts() {
	// return hearts;
	// }
	// void move(char side) {
	// }
	// void receivedDamage() {

	// }
	// void update() {

	// }
	/*draw() {

	}*/

