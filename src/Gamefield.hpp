#pragma once

#include "Game.hpp"



class Gamefield {
public:
	Gamefield(b2World* world);
	~Gamefield();	

	void draw(sf::RenderTarget& target);

private:
	//Sprite
	std::vector<sf::Sprite> mGroundSprites;
	std::map<std::string, sf::Texture> mGroundTextures;

	//Body
	b2Body* mGroundBody;
	b2BodyDef mGroundBodyDef;
	int GREENGROUNDBOX_WIDTH = 60;
	int GREENGROUNDBOX_HEIGHT = 30;

	void loadTexture(std::string filename);
};