#pragma once
#include <list>
#include <vector>
class Bullet {
private:
	int x_;
	int y_;
	int dx;
	int dy;
	char sprite;
	bool outrange;
public:
	Bullet() {
		x_ = 0;
		y_ = 0;
		sprite = { "" };
	}
	~Bullet() {
		sprite.clear();
	}
	setOutrange(bool valor) {
		outrange = valor;
	}
	void setX(int x) {
		x_ = x;
	}
	void setY(int y) {
		y_ = y;
	}
	int getX() {
		return x_;
	}
	int getY() {
		return y_;
	}
	getOutrange() {
		return outrange;
	}
	void move() {
	}
};

