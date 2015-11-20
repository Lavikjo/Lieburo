#ifndef PLAYER_HH
#define PLAYER_HH

#include "Game.hpp"
#include "Entity.hpp"
#include "Weapon.hpp"
class Weapon;


class Player : public Entity {
public:
	Player(Game* game);
	~Player();

	void movePlayer(float x, float y);
	void fire();

	virtual void update(sf::Time deltaTime);
	virtual void startContact();

private:
	float shootAngle = 0;
	bool shootFlag = false;
	std::vector<std::shared_ptr<Weapon>> mWeapons;
	int currentWeapon = 0;
};

#endif