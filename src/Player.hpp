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
	void addGorundContact();
	void removeGroundContact();

	sf::Vector2f getAimDotPosition();

	void update(sf::Time deltaTime);
	void startContact(int id);

	int getType();
	int getHp();

	sf::Vector2f returnPosition();

private:
	int hp = 100;
	float shootAngle = 150*DEG_TO_RAD;
	int direction = 1;//x-moving direction: either +1 or -1 
	std::vector<std::shared_ptr<Weapon>> mWeapons;
	int currentWeapon = 0;
	const float GUN_BARREL_LENGTH = 2.5f;//meters
	const float MIN_SHOOT_ANGLE = 30*DEG_TO_RAD;
	const float MAX_SHOOT_ANGLE = 180*DEG_TO_RAD;
	float previousXVelocity = 1;//at the beginning the moving direction is +x -> velocity > 0
	int numGroundContacts = 0;//used to check whether on ground ie. if the foot sensor is touching static bodies.  
};

#endif