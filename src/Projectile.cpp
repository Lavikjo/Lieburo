#include "Projectile.hpp"
#include "Shrapnel.hpp"

void Projectile::fragment(std::string texture, float maxLifeTime, float maxExplosionTime, float velocity, int numberOfFragments, float damage) {
	hasFragmented = true; //To not fragment multible times
	for(auto i = 0; i < numberOfFragments; i++) {
		//Create shrapnel
		std::shared_ptr<Shrapnel> s = std::make_shared<Shrapnel>(mGame, texture, maxLifeTime, maxExplosionTime, damage);
		mGame->getSceneNode()->attachChild(std::dynamic_pointer_cast<SceneNode>(s));

		//Make it fly
		s->setAlive(true);

		float a = (rand()%100)/100.0f;

		float angle = a*i; //The direction is random enough as i is radians.

		b2Body* body = s->getBody();
		float x = mBody->GetPosition().x+sin(angle+a)*a; 
		float y = mBody->GetPosition().y+cos(angle+a)*a;
		body->SetTransform(b2Vec2(x,y), 0);

		float velX = sin(angle)*velocity*a;
		float velY = cos(angle)*velocity*a;

		body->SetLinearVelocity(b2Vec2(velX, velY));
	}
}

Projectile::~Projectile(){
	mEntityWorld->DestroyBody(mBody);
}