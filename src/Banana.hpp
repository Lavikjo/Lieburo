#ifndef BANANA_HH
#define BANANA_HH

#include "Projectile.hpp"

#include "Game.hpp"

class Banana : public Projectile {
	public:
		Banana(Game* game);
		~Banana(){}

		virtual void update(sf::Time deltaTime);
		virtual void startContact();

};

#endif