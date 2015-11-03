#ifndef BULLET_HH
#define BULLET_HH

#include "Projectile.hpp"

class Bullet : public Projectile {
	public:
		Bullet();
		~Bullet();

		void update();
		void render(sf::RenderWindow &renderWindow);
};

#endif