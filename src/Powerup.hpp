#pragma once

#include "Entity.hpp"

class Powerup : public Entity{

public:
	Powerup(Game* game, std::string textureName,bool bodyTypeDynamic);
	~Powerup();

	bool isActive();
	void setActive(bool status);
private:
	bool active = false;
};