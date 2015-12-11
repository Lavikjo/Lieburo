#include "HealthRecovery.hpp"

void HealthRecovery::startContact(Entity* contact){
	if(typeid(*contact) == typeid(Player)){
		if (alive){
			contact->updateHp(20);
			setActive(true);
			mSprite.setColor(sf::Color(255,255,255,0));
		}
	}
}

void HealthRecovery::update(sf::Time deltaTime){
	if(alive && isActive()){
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > LIFETIME){
			setActive(false);
			mEntityWorld->DestroyBody(mBody);
		}
	}
	else lifeTime = 0; 
}
