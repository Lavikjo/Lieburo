#ifndef BANANA_HH
#define BANANA_HH

#include "Projectile.hpp"

#include "Game.hpp"
#include <iostream>

class Missile : public Projectile {
public:
	Missile(Game* game, int target);
	~Missile(){}

	void seek();

	virtual void update(sf::Time deltaTime);
	virtual void startContact(int id, Entity* contact);
	virtual int getType();
	int getTarget() const;
	void explode();
private:
	int mTarget;
	bool nearMiss = false;
	const float MISSILE_LIFETIME = 10.0f; // note: max value is number of missiles in the entity vector times trigger time.
	const int MISSILE_EXPLOSION_TIME = 15;
	const float MISSILE_MAX_VEL = 20;
};

#endif