#include "Banana.hpp"

Banana::Banana(Game* game) : Projectile(){

	mEntityWorld = game->getWorld(); 
	mGame = game;
	//Create the dynamic body
	mBodyDef.type = b2_dynamicBody;
	mBodyDef.position.Set(0, 0);
	mBodyDef.angle = 0;
	mBody = mEntityWorld->CreateBody(&mBodyDef);
	mBody->SetUserData(this);

	//Add a fixture to the body
	b2PolygonShape boxShape;
	boxShape.SetAsBox(1.0f,0.3f);
	mFixtureDef.shape = &boxShape;
	mFixtureDef.density = 5;
	mBody->CreateFixture(&mFixtureDef);

	// Declare and load a texture
	mTexture.loadFromFile("banana.png");
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	mSprite.setPosition(0, 0);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Initalize contact masks TODO...

	//Projectiles aren't alive by default
	alive = false;
}

void Banana::update(sf::Time deltaTime) {
	(void) deltaTime;
}

void Banana::startContact(){

}