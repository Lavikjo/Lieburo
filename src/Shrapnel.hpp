#pragma once
#include "Projectile.hpp"

#include "Game.hpp"
#include <iostream>

class Shrapnel : public Projectile {
public:
	Shrapnel(Game* game, std::string texture, float maxLifeTime, float maxExplosionTime);
	~Shrapnel(){}

	virtual void update(sf::Time deltaTime);
	virtual void startContact(int id, Entity* contact);
	virtual int getType();
private:
	float mMaxLifeTime;
	float mMaxExplosionTime;
};
