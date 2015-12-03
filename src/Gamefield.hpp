#pragma once

#include "Game.hpp"



class Gamefield :public Entity{
public:
	Gamefield(b2World* world);
	~Gamefield(){}	

	void draw(sf::RenderTarget& target);
	virtual int getType();

	virtual void update(sf::Time){}
	virtual void startContact(int, Entity*){}

private:
	//Sprite
	std::vector<sf::Sprite> mGroundSprites;
	std::map<std::string, sf::Texture> mGroundTextures;

	int GREENGROUNDBOX_WIDTH = 60;
	int GREENGROUNDBOX_HEIGHT = 30;

	void loadTexture(std::string filename);
};