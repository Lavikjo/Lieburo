#include "Banana.hpp"

Banana::Banana(Game* game) : Projectile(){

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
			explosionClock++;

			if(!hasFragmented) {
				hasFragmented = true;
				for(auto i = 0; i < 5; i++) {

				//Create shrapnel
				std::shared_ptr<Shrapnel> s = std::make_shared<Shrapnel>(mGame, "texture/minibanana.png", 1.0f, 15.0f);
				mGame->getSceneNode()->attachChild(std::dynamic_pointer_cast<SceneNode>(s));

				//Make it fly
				s->setAlive(true);
				s->getBody()->GetFixtureList()[0].SetSensor(false); //bananas have only 1 fixture
				b2Body* body = s->getBody();
				body->SetTransform(mBody->GetPosition(), 0);

				float angle = i;

		    	float velX = sin(angle)*20;
		    
		    	float velY = cos(angle)*20;
		    
		    	body->SetLinearVelocity(b2Vec2(velX, velY));
			}
			}
		}
		else{
			exploses = false;
			lifeTime = MAX_LIFETIME +1; //it will die next.
		}
	}
	
}

void Banana::startContact(int id, Entity* contact){
	if(typeid(*contact) == typeid(Player)) {
		if(alive) {
			contact->updateHp(-10);
			mSprite.setTexture(mExplosionTexture, true); //true resets the sprite boundaries
			exploses = true;
			explosionClock = 0;
		}
	}
	
	(void) id;
}

int Banana::getType() {
	return BANANA;
}