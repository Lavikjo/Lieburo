#ifndef BANANA_HH
#define BANANA_HH

#include "Projectile.hpp"

#include "Game.hpp"
#include <iostream>

class Banana : public Projectile {
public:
	Banana(Game* game);
	~Banana(){}

	virtual void update(sf::Time deltaTime);
	virtual void startContact(int id);
	virtual int getType();
private:
	const float MAX_LIFETIME = 2.5f;
	const int BANANA_EXPLOSION_TIME = 15;
};

#endif