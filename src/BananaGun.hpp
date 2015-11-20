#ifndef BANANAGUN_HH
#define BANANAGUN_HH

#include "Weapon.hpp"
#include "Game.hpp"
class Banana;

class BananaGun : public Weapon {
public:
	BananaGun(Game* game);

	virtual void shoot(float angle, b2Vec2 position,Game* game);
};

#endif