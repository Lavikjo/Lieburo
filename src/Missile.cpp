#include "Missile.hpp"

Missile::Missile(Game* game, int target){


	baseConstructor(game, "texture/aim_120c.png");
	mTarget = target;

	//The explosion texture and clock
	mExplosionTexture.loadFromFile("texture/banana_explosion.png");
	explosionTime = MISSILE_EXPLOSION_TIME;

	//Projectiles aren't alive by default
	alive = false;
}

void Missile::update(sf::Time deltaTime) {
	if(alive && !exploses) {
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > MISSILE_LIFETIME) {
			alive = false;
			mSprite.setTexture(mTexture, true);

		}
		seek();
	}else{
		lifeTime = 0;
	}

	if(exploses ){
		if(explosionClock < explosionTime){
			mBody->SetLinearVelocity(b2Vec2(0,0));//We want the explosion to be stationary(?)
			mBody->SetAngularVelocity(0);
			explosionClock+= deltaTime.asSeconds();

			if(!hasFragmented) {
				fragment("texture/bullet.png", 0.2f, 0.25f, 30.0f, 12, 1.0f);
				
			}
		}
		else{
			exploses = false;
			
			lifeTime = MISSILE_LIFETIME +1; //it will die next.
		}
	}
	
}

void Missile::startContact(Entity* contact){ 
	if(lifeTime > 0.2) {//Preventing barrel explosion
		if(typeid(*contact) == typeid(Player)) {
			contact->updateHp(-4);
			lifeTime = MISSILE_LIFETIME +1;
			hasFragmented = true;
		}
		explode();
	}

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
	if(lifeTime < 0.2f) {
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
		startContact(std::dynamic_pointer_cast<Entity>(mGame->getPlayer(mTarget)).get());
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