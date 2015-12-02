#include "Bomb.hpp"
//	#include <stdlib.h> 

void Bomb::startContact(Entity* contact){
	if(typeid(*contact) == typeid(Player)){
		if(alive){
			bombExplosion();
			mSprite.setColor(sf::Color(255,255,255,0));
			setActive(true);
			contact->updateHp(-20);
		}
	}
}

void Bomb::update(sf::Time deltaTime){
	if(alive && isActive()){
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > LIFETIME){
			setActive(false);
			mEntityWorld->DestroyBody(mBody);
		}
	}
	else
		lifeTime = 0;
}

void Bomb::bombExplosion(){
	b2Vec2 uBound = mBody->GetPosition();
	b2Vec2 lBound = uBound;
	uBound.x += 3;
	uBound.y += 3;
	lBound.x -= 3;
	lBound.y -= 3;

	MyQueryCallback queryCallback;
	b2AABB aabb;

	aabb.upperBound = uBound;
	aabb.lowerBound = lBound;

	mEntityWorld->QueryAABB(&queryCallback, aabb);

	for(unsigned int i = 0; i < queryCallback.foundBodies.size();i++){
		Player* player = static_cast<Player*>(queryCallback.foundBodies[i]->GetUserData());
		player->disableUserInput(true);
		b2Vec2 bodyPos = queryCallback.foundBodies[i]->GetPosition();
		bodyPos.x = (1800-bodyPos.x)/2;
		bodyPos.y = (1200-bodyPos.y)/2;
		b2Vec2 bodyCenter = queryCallback.foundBodies[i]->GetWorldCenter();
		queryCallback.foundBodies[i]->ApplyLinearImpulse(bodyPos,bodyCenter);
		player->disableUserInput(false);


	}


}