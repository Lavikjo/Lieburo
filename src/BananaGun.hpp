#ifndef BANANAGUN_HH
#define BANANAGUN_HH

#include "Weapon.hpp"
#include "Game.hpp"
class Banana;

class BananaGun : public Weapon {
public:
	BananaGun(Game* game);

	virtual void shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game);
};

#endif