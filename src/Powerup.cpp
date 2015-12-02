#include "Powerup.hpp"
#include "Game.hpp"

Powerup::Powerup(Game* game, std::string textureName, bool bodyTypeDynamic){
	alive = true;
	mEntityWorld = game->getWorld(); 
	mGame = game;
	//Create the dynamic body
	if(bodyTypeDynamic)
		mBodyDef.type = b2_dynamicBody;
	else
		mBodyDef.type = b2_staticBody;
	
	mBodyDef.position.Set((rand()/(int)PIXELS_PER_METER)%(GAMEFIELD_WIDTH/(int)PIXELS_PER_METER),
	(rand()/(int)PIXELS_PER_METER)%(GAMEFIELD_HEIGHT/(int)PIXELS_PER_METER));
	mBodyDef.angle = 0;
	mBody = mEntityWorld->CreateBody(&mBodyDef);
	mBody->SetUserData(this);
	
	// Declare and load a texture
	mTexture.loadFromFile(textureName);
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Add a fixture to the body
	b2PolygonShape boxShape;
	boxShape.SetAsBox(0.5f*bounds.width / PIXELS_PER_METER, 0.5f*bounds.height/PIXELS_PER_METER);
	mFixtureDef.shape = &boxShape;
	mFixtureDef.density = 1;

	mBody->CreateFixture(&mFixtureDef);
}

Powerup::~Powerup(){}

bool Powerup::isActive(){
	return active;
}

void Powerup::setActive(bool status){
	active = status;
}



