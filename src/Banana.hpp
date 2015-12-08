#ifndef BANANA_HH
#define BANANA_HH

#include "Projectile.hpp"
#include "Shrapnel.hpp"
#include "Game.hpp"
#include <iostream>

class Banana : public Projectile {
public:
	Banana(Game* game);
	~Banana(){}

	virtual void update(sf::Time deltaTime);
	virtual void startContact(Entity* contact);

	void explode();

private:
	const float MAX_LIFETIME = 2.5f;
	const float BANANA_EXPLOSION_TIME = 0.25f;
};

#endif