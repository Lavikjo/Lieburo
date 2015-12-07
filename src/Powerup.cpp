#include "Powerup.hpp"

Powerup::Powerup(Game* game, std::string textureName){
	alive = true;
	mEntityWorld = game->getWorld(); 
	mGame = game;
	//Create the dynamic body
	mBodyDef.type = b2_staticBody;
	mBodyDef.position.Set(500 / PIXELS_PER_METER,50 / PIXELS_PER_METER);
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

Powerup::~Powerup(){
	mEntityWorld->DestroyBody(mBody);
}

void Powerup::changeGravity(Game* game){
	mEntityWorld->SetGravity(b2Vec2(0.0, 0));
}

void Powerup::update(sf::Time deltaTime) {
	(void) deltaTime;
}

void Powerup::startContact(Entity* contact){
	if(typeid(*contact) == typeid(Player)){
		if(alive)
			changeGravity(mGame);
	}
}

