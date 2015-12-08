#include "Projectile.hpp"
#include "Shrapnel.hpp"

void Projectile::fragment(std::string texture, float maxLifeTime, float maxExplosionTime, float velocity, int numberOfFragments, float damage) {
	hasFragmented = true;
	for(auto i = 0; i < numberOfFragments; i++) {
		//Create shrapnel
		std::shared_ptr<Shrapnel> s = std::make_shared<Shrapnel>(mGame, texture, maxLifeTime, maxExplosionTime, damage);
		mGame->getSceneNode()->attachChild(std::dynamic_pointer_cast<SceneNode>(s));

		//Make it fly
		s->setAlive(true);
		b2Body* body = s->getBody();
		body->SetTransform(mBody->GetPosition(), 0);


		float angle = i; //The direction is random enough as i is radians.
		float velX = sin(angle)*velocity;
		float velY = cos(angle)*velocity;

		body->SetLinearVelocity(b2Vec2(velX, velY));
	}
}