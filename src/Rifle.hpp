#pragma once

#include "Weapon.hpp"
#include "Game.hpp"

class Rifle : public Weapon {
public:
	Rifle(Game* game);

	virtual void shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game);
};
