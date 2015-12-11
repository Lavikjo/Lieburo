#include "GravityPU.hpp"


void GravityPU::startContact(Entity* contact){
	if(typeid(*contact) == typeid(Player)){
		if(alive){
			changeGravity(2.0f);
			mSprite.setColor(sf::Color(255,255,255,0));
			setActive(true);
		}
	}
}

void GravityPU::update(sf::Time deltaTime) {
	if(alive && isActive()){
		lifeTime += deltaTime.asSeconds();	
		if(lifeTime > LIFETIME){
			changeGravity(9.8f);
			setActive(false);
			mEntityWorld->DestroyBody(mBody);
		}
	}
		else
			lifeTime = 0;
}

void GravityPU::changeGravity(float gravityValue){

	mEntityWorld->SetGravity(b2Vec2(0, gravityValue));
}