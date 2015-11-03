#ifndef PROJECTILE_HH
#define PROJECTILE_HH

#include "Entity.hpp"

class Projectile : public Entity {
public:
	
	enum Type {
		Bullet,
		Nuke,
	};

	explicit Projectile(Type type);

private:
	Type mType;
	bool alive;


};


#endif