#pragma once

#include "Powerup.hpp"
#include "Game.hpp"

class GravityPU : public Powerup {
public:
	using Powerup::Powerup;	
	void startContact(Entity* contact);
	void update(sf::Time deltaTime);
	void changeGravity();
	void changeGravity(float gravityValue);
	
private:
	float LIFETIME = 15;
};