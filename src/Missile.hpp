#pragma once

#include "Projectile.hpp"

#include "Game.hpp"
#include <iostream>

class Missile : public Projectile {
public:
	Missile(Game* game, int target);
	~Missile(){}

	void seek();

	virtual void update(sf::Time deltaTime);
	virtual void startContact(Entity* contact);
	int getTarget() const;
	void explode();
private:
	int mTarget;
	bool nearMiss = false;
	const float MISSILE_LIFETIME = 10.0f; 
	const float MISSILE_EXPLOSION_TIME = 0.25f;
	const float MISSILE_MAX_VEL = 20;
};
