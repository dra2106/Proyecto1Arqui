#pragma once
#include <list>
#include <vector>
class Spaceship {
private:
	int xLoc, yLoc, xMax, yMax; // Loc es location
	char character;
	WINDOW* currentWindow; // la ventana donde se muestra la nave
	//int hearts;
	//bool inmune;
	//int cooldown;
	//std::list <std::string> sprite;
public:
	Spaceship(WINDOW* win, int y, int x, char c) {
		currentWindow = win;
		yLoc = y;
		xLoc = x;
		getmaxyx(currentWindow, yMax, xMax);
		keypad(currentWindow, true); // esto es para activar el uso de las flechas
		character = c;
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

	void moveLeft() {
		mvwaddch(currentWindow, yLoc, xLoc, ' '); // esto es para no dejar caracteres por donde se mueva
		xLoc--;
		// esta parte es para que no se salga de la caja
		if (xLoc < 1)
			xLoc = 1;
	}

	void moveRight() {
		mvwaddch(currentWindow, yLoc, xLoc, ' ');
		xLoc++;
		// esta parte es para que no se salga de la caja
		if (xLoc > xMax-2)
			xLoc = xMax-2;
	}

	int getMove() {
		int choice = wgetch(currentWindow);
		switch(choice) {
			case KEY_UP: // por ahora, arriba no hace nada
				break;
			case KEY_DOWN: // abajo no hace nada
				break;
			case KEY_LEFT:
				moveLeft();
				break;
			case KEY_RIGHT:
				moveRight();
				break;
			default:
				break;
		}
		return choice;
	}

	void display() {
		mvwaddch(currentWindow, yLoc, xLoc, character);
	}

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
};
