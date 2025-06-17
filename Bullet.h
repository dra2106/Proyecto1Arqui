#pragma once

#include "Entity.h"
#include "Spaceship.h"

class Bullet : public Entity{
private:
	Direction currentDirection;
	// int x_;
	// int y_;
	// int dx;
	// int dy;
	// char sprite;
	// bool outrange;
public:
	Bullet() {
		this->x = 0;
		this->y = 0;
	}

	Bullet(int y, int x) {
		this->x = x;
		this->y = y;
		icon = '|';
	}

	void move() {
		if (currentDirection == up)
			y--;
	}

	void setDirection(Direction d) {
		currentDirection = d;
	}
	// ~Bullet() {
	// 	sprite.clear();
	// }
	// setOutrange(bool valor) {
	// 	outrange = valor;
	// }
	// void setX(int x) {
	// 	x_ = x;
	// }
	// void setY(int y) {
	// 	y_ = y;
	// }
	// int getX() {
	// 	return x_;
	// }
	// int getY() {
	// 	return y_;
	// }
	// getOutrange() {
	// 	return outrange;
	// }
	// void move() {
	// }
};

