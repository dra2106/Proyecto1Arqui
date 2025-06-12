// clase abstracta (idea)

#pragma once

class Enemy {
private:
	Enemy(const Enemy& other) {}
	void operator=(const Enemy&) {}

public:
	Enemy() {}
	virtual ~Enemy() {}
	virtual void move() = 0;
	virtual void draw() = 0;
	virtual void shoot() = 0;
	virtual void update() = 0;
	virtual void kill() = 0;
};