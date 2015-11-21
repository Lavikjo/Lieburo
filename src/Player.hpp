#ifndef PLAYER_HH
#define PLAYER_HH

#include "Game.hpp"
#include "Entity.hpp"
#include "Weapon.hpp"
#include "Constants.h"
class Weapon;


class Player : public Entity {
public:
	Player(Game* game);
	~Player();

	void movePlayerX(float x);
	void movePlayerY(float y);
	void fire();
	void aim(float angleChange);

	sf::Vector2f getAimDotPosition();

	virtual void update(sf::Time deltaTime);
	virtual void startContact();

private:
	float shootAngle = 150*DEG_TO_RAD;
	int direction = 1;//x-moving direction: either +1 or -1 
	std::vector<std::shared_ptr<Weapon>> mWeapons;
	int currentWeapon = 0;
	const float GUN_BARREL_LENGTH = 3.0f;//meters
	const float MIN_SHOOT_ANGLE = 60*DEG_TO_RAD;
	const float MAX_SHOOT_ANGLE = 180*DEG_TO_RAD;
	float previousXVelocity = 1;//at the beginning the moving direction is +x -> velocity > 0
};

#endif