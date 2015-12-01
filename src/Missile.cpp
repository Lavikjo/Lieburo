#include "Missile.hpp"

Missile::Missile(Game* game, int target) : Projectile(){

	mEntityWorld = game->getWorld(); 
	mGame = game;
	mTarget = target;
	//Create the dynamic body
	mBodyDef.type = b2_dynamicBody;
	mBodyDef.position.Set(0, 0);
	mBodyDef.angle = 0;
	mBody = mEntityWorld->CreateBody(&mBodyDef);
	mBody->SetUserData(this);

	//Add a fixture to the body
	b2PolygonShape boxShape;
	boxShape.SetAsBox(0.1f,0.4f);
	mFixtureDef.shape = &boxShape;
	mFixtureDef.isSensor = true;
	mFixtureDef.density = 1;

	mBody->CreateFixture(&mFixtureDef);

	// Declare and load a texture
	mTexture.loadFromFile("aim_120c.png");
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	mSprite.setPosition(0, 0);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//The explosion texture and clock
	mExplosionTexture.loadFromFile("banana_explosion.png");
	explosionTime = MISSILE_EXPLOSION_TIME;

	//Projectiles aren't alive by default
	alive = false;
}

void Missile::update(sf::Time deltaTime) {
	if(alive && !exploses) {
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > MISSILE_LIFETIME) {
			alive = false;
			this->getBody()->GetFixtureList()[0].SetSensor(true);
			return;
		}
		seek();
	}else{
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
			lifeTime = MISSILE_LIFETIME +1; //it will die next.
		}
	}
	
}

void Missile::startContact(int id, Entity* contact){
	if(typeid(*contact) == typeid(Player)) {
		if(alive) {
			contact->updateHp(-20);
		}
	}
	explode();

	(void) id;
}

int Missile::getType() {
	return MISSILE;
}

void Missile::seek(){
	b2Vec2 mPos = mBody->GetPosition();
	b2Vec2 tPos = mGame->getPlayer(mTarget)->getBody()->GetPosition();//->isAlive();//getBody()->GetPosition();

	float x = tPos.x-mPos.x;
	float y = tPos.y-mPos.y;
	float dist = sqrt(x*x + y*y);

	// Fine-tuning the control algorithm
	if(x > 0){
		x = .7f;
	}else if(x < 0){
		x = -.7f;
	}
	
	if(y > 0){
		y = .7f;
	}else if( y < 0) {
		y = -.7f;
	}

	//slow at first second(s)
	if(lifeTime < 1.0f) {
		x *= 0.1f;
		y *= 0.1f;
	}

	//able to dodge
	if(dist < 4.0f){//use the lifetime in order to prevent near-launch errors
		x = 0;
		y = 0;
		
	}
	//successfull dodge will pay off.
	if(dist < 2.5f && lifeTime > 1.0f){ //use the lifetime in order to prevent near-launch errors
		nearMiss = true;
	}
	else if(nearMiss && dist > 3.0f ) { 
		explode();
	}

	//check near hit
	if(dist < 1.0f) {
		startContact(0, std::dynamic_pointer_cast<Entity>(mGame->getPlayer(mTarget)).get());
	}


	//Applying the forces
	mBody->ApplyLinearImpulse( b2Vec2(x, y), mBody->GetWorldCenter() );

	//Aiming
	b2Vec2 mVel = mBody->GetLinearVelocity();
	mBody->SetTransform(mPos, atan2(-mVel.x, mVel.y));

	//Limiting the speed
	float ratio = MISSILE_MAX_VEL/sqrt(mVel.x*mVel.x + mVel.y*mVel.y);
	if(ratio < 1) {
		mBody->SetLinearVelocity({mVel.x*ratio, mVel.y*ratio});
	}
	
}
int Missile::getTarget() const{
	return mTarget;
}

void Missile::explode(){ //terminates the cycle, it will die after explosion.
	mSprite.setTexture(mExplosionTexture, true); //true resets the sprite boundaries
	exploses = true;
	explosionClock = 0;
	nearMiss = false;
}