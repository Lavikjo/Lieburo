#include "Entity.hpp"
#include "Player.hpp"
#include "Constants.h"

void Entity::draw(sf::RenderTarget& target){
	if(alive) {

		if(typeid(*this) == typeid(Player)) { // draw the player extras so that the player sprite will be in front of them
			this->drawPlayer(target);
			//healthBar.setPosition(mBody->GetPosition().x*PIXELS_PER_METER, mBody->GetPosition().y*PIXELS_PER_METER - 40);
			//target.draw(healthBar);
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

b2Body* Entity::getBody() const{ 
	return mBody;
}

bool Entity::isAlive(){
	return alive;
}

void Entity::setAlive(bool status){
	alive = status;
}

sf::Sprite Entity::getSprite(){
	return mSprite;
}

