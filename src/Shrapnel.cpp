#include "Shrapnel.hpp"

Shrapnel::Shrapnel(Game* game, std::string texture, float maxLifeTime, float maxExplosionTime = 0) : Projectile(){

	baseConstructor(game, texture);

	//The explosion texture and clock
	mExplosionTexture.loadFromFile("texture/banana_explosion.png");
	mMaxExplosionTime = maxExplosionTime;
	mMaxLifeTime = maxLifeTime;
	//Projectiles aren't alive by default
	alive = false;
}

void Shrapnel::update(sf::Time deltaTime) {

	if(alive && !exploses) {
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > mMaxLifeTime) {
			alive = false;
			mSprite.setTexture(mTexture, true);
		}
	}else {
		lifeTime = 0;
	}

	if(exploses ){
		if(explosionClock < mMaxExplosionTime){
			mBody->SetLinearVelocity(b2Vec2(0,0));//We want the explosion to be stationary(?)
			mBody->SetAngularVelocity(0);
			explosionClock++;
		}
		else{
			exploses = false;
			lifeTime = mMaxLifeTime +1; //it will die next.
		}
	}
	
}

void Shrapnel::startContact(int id, Entity* contact){
	if(typeid(*contact) == typeid(Player)) {
		if(alive && lifeTime > 0.1f) {
			contact->updateHp(-10);
			mSprite.setTexture(mExplosionTexture, true); //true resets the sprite boundaries
			exploses = true;
			explosionClock = 0;
		}
	}

	(void) id;
}

int Shrapnel::getType() {
	return BANANA;
}