#pragma once

#include "Weapon.hpp"
#include "Game.hpp"

class MissileLauncher : public Weapon {
public:
	MissileLauncher(Game* game, int target);

	virtual void shoot(float, b2Vec2, b2Vec2, Game*);

private:
	int mTarget;
};
