#pragma once

#include "Powerup.hpp"
#include "Game.hpp"

class HealthRecovery : public Powerup{
public:
	using Powerup::Powerup;
	void startContact(Entity* contact);
	void update(sf::Time deltaTime);

private:
	float LIFETIME = 0.1;	

};