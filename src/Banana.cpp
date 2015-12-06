#include "Banana.hpp"

Banana::Banana(Game* game){

	baseConstructor(game, "texture/banana.png");

	//The explosion texture and clock
	mExplosionTexture.loadFromFile("texture/banana_explosion.png");
	explosionTime = BANANA_EXPLOSION_TIME;

	//Projectiles aren't alive by default
	alive = false;
}

void Banana::update(sf::Time deltaTime) {

	if(alive && !exploses) {
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > MAX_LIFETIME) {
			alive = false;
			mSprite.setTexture(mTexture, true);
		}
	}else {
		lifeTime = 0;
	}

	if(exploses ){
		if(explosionClock < explosionTime){
			mBody->SetLinearVelocity(b2Vec2(0,0));//We want the explosion to be stationary(?)
			mBody->SetAngularVelocity(0);
			explosionClock += deltaTime.asSeconds();

			if(!hasFragmented) {
				fragment("texture/minibanana.png", 2.0f, 0.25f, 20.0f, 5, 1.0);
				
			}
		}
		else{
			exploses = false;
			lifeTime = MAX_LIFETIME +1; //it will die next.
		}
	}
	
}

void Banana::startContact(Entity* contact){
	if(typeid(*contact) == typeid(Player)) {
		if(alive) {
			contact->updateHp(-10);
			mSprite.setTexture(mExplosionTexture, true); //true resets the sprite boundaries
			exploses = true;
			explosionClock = 0;
		}
	}
	
}

