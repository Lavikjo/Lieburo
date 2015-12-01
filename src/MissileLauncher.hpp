#pragma once

#include "Weapon.hpp"
#include "Game.hpp"

class MissileLauncher : public Weapon {
public:
	MissileLauncher(Game* game, int target);

	virtual void shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game);

private:
	int mTarget;
};
