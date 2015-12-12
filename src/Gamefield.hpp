#pragma once

#include "Entity.hpp"
#include "Constants.h"



class Gamefield :public Entity{
public:
	Gamefield(b2World* world);
	~Gamefield(){}	

	void draw(sf::RenderTarget& target);

	virtual void update(sf::Time){}
	virtual void startContact(int, Entity*){}

private:
	//Sprite
	std::vector<sf::Sprite> mGroundSprites;
	std::map<std::string, sf::Texture> mGroundTextures;

	void loadTexture(std::string filename);
};