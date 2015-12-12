#include "Entity.hpp"
#include "Player.hpp"
#include "Constants.h"

void Entity::draw(sf::RenderTarget& target){
	if(alive) {

		if(typeid(*this) == typeid(Player)) { // draw the player extras so that the player sprite will be in front of them
			this->drawPlayer(target);
			//healthBar.setPosition(mBody->GetPosition().x*PIXELS_PER_METER, mBody->GetPosition().y*PIXELS_PER_METER - 40);
			//target.draw(healthBar);

			return;
		}

		//Synchronize sprite coordinates with body
		b2Vec2 pos = mBody->GetPosition();
		spriteAngle = mBody->GetAngle() * RAD_TO_DEG;
		spritePosition.x = pos.x * PIXELS_PER_METER;
		spritePosition.y = pos.y * PIXELS_PER_METER;

		//Draw Sprite
		mSprite.setPosition(spritePosition);
		mSprite.setRotation(spriteAngle);
		target.draw(mSprite);
	}
}

b2Body* Entity::getBody() const { 
	return mBody;
}

bool Entity::isAlive() {
	return alive;
}

void Entity::setAlive(bool status){
	alive = status;
}

sf::Sprite Entity::getSprite() const {
	return mSprite;
}

void Entity::disableUserInput(bool status){
	(void)status;
}

void Entity::baseConstructor(Game* game, std::string textureName) {
	mEntityWorld = game->getWorld(); 
	mGame = game;
	//Create the dynamic body
	mBodyDef.type = b2_dynamicBody;
	mBodyDef.position.Set(0, 0);
	mBodyDef.angle = 0;
	mBody = mEntityWorld->CreateBody(&mBodyDef);
	mBody->SetUserData(this);

	// Declare and load a texture
	mTexture.loadFromFile(textureName);
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	mSprite.setPosition(0, 0);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Add a fixture to the body
	b2PolygonShape boxShape;
	boxShape.SetAsBox(0.5f*bounds.width / PIXELS_PER_METER, 0.5f*bounds.height/PIXELS_PER_METER);
	mFixtureDef.shape = &boxShape;
	mFixtureDef.density = 1;

	mBody->CreateFixture(&mFixtureDef);
}

