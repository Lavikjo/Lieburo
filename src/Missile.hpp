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
private:
	int mTarget;
	const float MISSILE_LIFETIME = 20.0f;
	const int MISSILE_EXPLOSION_TIME = 15;
	const float MISSILE_MAX_VEL = 14;
};

#endif