#pragma once

#include "Projectile.hpp"

#include "Game.hpp"
#include <iostream>

class Bullet : public Projectile {
public:
	Bullet(Game* game);

	virtual void update(sf::Time deltaTime);
	virtual void startContact(Entity* contact);

private:
	const float MAX_LIFETIME = 2.5f;

};
