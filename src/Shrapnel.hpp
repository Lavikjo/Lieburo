#pragma once
#include "Projectile.hpp"

#include "Game.hpp"
#include <iostream>

class Shrapnel : public Projectile {
public:
	Shrapnel(Game* game, std::string texture, float maxLifeTime, float maxExplosionTime, float damage);
	~Shrapnel(){}

	virtual void update(sf::Time deltaTime);
	virtual void startContact(Entity* contact);
private:
	float mMaxLifeTime;
	float mMaxExplosionTime;
};
