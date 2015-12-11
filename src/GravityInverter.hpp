#pragma once

#include "Powerup.hpp"
#include "Game.hpp"

class GravityInverter : public Powerup {
public:
	using Powerup::Powerup;
	//GravityInverter(Game* game, std::string textureName, bool bodyTypeDynamic, float gravValue);	
	//~GravityInverter(){}
	void startContact(Entity* contact);
	void update(sf::Time deltaTime);
	void changeGravity(float gravityValue);
	
private:
	float LIFETIME = 5;
	float gravityValue = -9.8f;
};