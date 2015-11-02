#ifndef PROJECTILE_HH
#define PROJECTILE_HH

#include "Entity.hpp"

class Projectile : public Entity {
public:

	virtual void update() = 0;
	virtual void render() = 0;
	virtual void logic() = 0;	

protected:
	bool alive;


};


#endif