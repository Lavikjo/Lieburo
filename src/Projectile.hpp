#ifndef PROJECTILE_HH
#define PROJECTILE_HH

#include "Entity.hpp"

class Projectile : public Entity {
public:
	Projectile() = default;
	~Projectile(){mEntityWorld->DestroyBody(mBody);}

	virtual void update(sf::Time) = 0;
	void fragment(std::string, float, float, float, int, float);

protected:
	sf::Sprite mExplosionSprite;
	sf::Texture mExplosionTexture;
	bool exploses = false;
	float explosionClock = 0;
	float explosionTime;
	bool hasFragmented = false;
	float hitDamage;//Used to set the damage to player

};


#endif