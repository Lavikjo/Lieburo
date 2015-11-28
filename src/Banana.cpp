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
	boxShape.SetAsBox(0.5f,0.2f);
	mFixtureDef.shape = &boxShape;
	mFixtureDef.isSensor = true;
	mFixtureDef.density = 1;

	mBody->CreateFixture(&mFixtureDef);

	// Declare and load a texture
	mTexture.loadFromFile("banana.png");
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	mSprite.setPosition(0, 0);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//The explosion texture and clock
	mExplosionTexture.loadFromFile("banana_explosion.png");
	explosionTime = BANANA_EXPLOSION_TIME;

	//Projectiles aren't alive by default
	alive = false;
}

void Banana::update(sf::Time deltaTime) {
	if(alive & !exploses) {
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > MAX_LIFETIME) {
			alive = false;
			this->getBody()->GetFixtureList()[0].SetSensor(true);
		}
	}else {
		lifeTime = 0;
	}

	if(exploses ){
		if(explosionClock < explosionTime){
			mBody->SetLinearVelocity(b2Vec2(0,0));//We want the explosion to be stationary(?)
			mBody->SetAngularVelocity(0);
			explosionClock++;
		}
		else{
			exploses = false;
			mSprite.setTexture(mTexture, true);
			lifeTime = MAX_LIFETIME +1; //it will die next.
		}
	}
	
}

void Banana::startContact(int id){
	if(id == PLAYER) {
		if(alive) {
			mSprite.setTexture(mExplosionTexture, true); //true resets the sprite boundaries
			exploses = true;
			explosionClock = 0;
		}
	}
}

int Banana::getType() {
	return BANANA;
}