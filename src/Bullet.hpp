#pragma once

#include "Projectile.hpp"

#include "Game.hpp"
#include <iostream>

class Bullet : public Projectile {
public:
	Bullet(Game* game);
	~Bullet(){}

	virtual void update(sf::Time deltaTime);
	virtual void startContact(int id, Entity* contact);
	virtual int getType();
private:
	const float MAX_LIFETIME = 2.5f;

};
