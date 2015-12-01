#ifndef PLAYER_HH
#define PLAYER_HH

#include "Game.hpp"
#include "Entity.hpp"
#include "Weapon.hpp"
#include "Constants.h"
class Weapon;



class Player : public Entity {
public:
	Player(Game* game, int opponent);
	~Player();

	void movePlayerX(float x);
	void jump();
	void fire();
	void aim(float angleChange);
	void updateGroundContacts(int val);

	sf::Vector2f getAimDotPosition() const;

	void update(sf::Time deltaTime);
	void startContact(int id, Entity* contact);

	virtual int getType();
	virtual void drawPlayer(sf::RenderTarget& target);
	int getHp() const;
	float getFuel() const;

	unsigned int getCurrentAmmo() const;
	unsigned int getCurrentClipSize() const;
	virtual void updateHp(int val);

	sf::Vector2f returnPosition();
	void scrollWeapons();

private:
	int mOpponent;
	//possible aim dot, jetpack etc. (players only)
	sf::Texture aimDotTexture;
	sf::Sprite aimDotSprite;
	sf::Texture jetpackTexture;
	sf::Sprite jetpackSprite;
	sf::Texture bazookaTexture;
	sf::Sprite bazookaSprite;

	int hp = 100;
	float jetpackFuel = 100;
	bool jetpackReady = true;
	int jetpackTimer = 0;//a timer-like counter used to extinquish the jetpack flame 
	int jumpTimer = 0; // timer for igniting the jetpack after jump.
	float shootAngle = 150*DEG_TO_RAD;
	int direction = 1;//x-moving direction: either +1 or -1 
	int gunDirection = 1; //used for setting the direction of the gun
	std::vector<std::shared_ptr<Weapon>> mWeapons;
	unsigned int currentWeapon = 0;
	const float GUN_BARREL_LENGTH = 1.5f;//meters
	const float MIN_SHOOT_ANGLE = 30*DEG_TO_RAD;
	const float MAX_SHOOT_ANGLE = 180*DEG_TO_RAD;
	float previousXVelocity = 1;//at the beginning the moving direction is +x -> velocity > 0
	int numGroundContacts = 0;//used to check whether on ground ie. if the foot sensor is touching static bodies.  
	const int PLAYER_JUMP_SPEED = -7;//negative for up
	const float JETPACK_THRUST = -0.15;
	const float MIN_JETPACK_RELOAD_FUEL = 50;
	const float JETPACK_MAX_FUEL = 100;
	const float JETPACK_FUEL_CONSUMPTION = 0.4f;
	const float JETPACK_FUEL_FILL = 0.05f;
	const float MAX_JETPACK_SPEED = 10;
};

#endif