#pragma once

#include "Powerup.hpp"
#include "Game.hpp"
#include "MyQueryCallback.hpp"

class MyQueryCallback;
class Bomb : public Powerup {
public:
	using Powerup::Powerup;
	void startContact(Entity* contact);
	void update(sf::Time deltaTime);
	void bombExplosion();

private:
	float LIFETIME = 0.5f;
};