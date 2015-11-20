#ifndef PROJECTILE_HH
#define PROJECTILE_HH

#include "Entity.hpp"

class Projectile : public Entity {
public:
	Projectile() = default;
	~Projectile(){}

	//virtual void update(sf::Time deltaTime) = 0;
	//virtual void startContact() = 0;

};


#endif