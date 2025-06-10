#pragma once
#include <list>
#include <vector>
class Spaceship {
private:
	int x_;
	int y_;
	int hearts;
	bool inmune;
	int cooldown;
	std::list <std::string> sprite;
public:
	Spaceship() {
		x_ = 0;
		y_ = 0;
		hearts = 3;
		inmune = false;
		cooldown = 0;
		sprite = { "" };
	}
	~Spaceship() {
		sprite.clear();
	}

	void setX(int x) {
		x_ = x;
	}
	void setY(int y) {
		y_ = y;
	}
	int getX(){
		return x_;
	}
	int getY() {
		return y_;
	}
	void setInmune(bool inm) {
		inmune = inm;
	}
	void setCooldown(int cooldown) {
		this->cooldown = cooldown;
	}
	void setHearts(int hearts) {
		this->hearts = hearts;
	}
	bool isInmune() {
		return inmune;
	}
	int getCooldown(){
		return cooldown;
	}
	int getHearts() {
	return hearts;
	}
	void move(char side) {
	}
	void receivedDamage() {

	}
	void update() {

	}
	/*draw() {

	}*/
};
