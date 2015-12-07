#pragma once

#include "Entity.hpp"
#include "Game.hpp"

class Powerup : public Entity{

public:
	Powerup(Game* game, std::string textureName);
	~Powerup();

	void changeGravity(Game* game,float gravityValue);
	void startContact(Entity* contact);
	void update(sf::Time deltaTime);
private:
	float LIFETIME = 15;
};