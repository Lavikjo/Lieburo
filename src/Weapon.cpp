#include "Weapon.hpp"

Weapon::Weapon(unsigned int clipSize, float fireRate, float reloadTime, float muzzleVelocity, Game* game)
	:clipSize(clipSize), fireRate(fireRate), reloadTime(reloadTime), mGame(game), mGameWorld(game->getWorld()){
		ammo = clipSize;
	}

void Weapon::launchProjectile(float angle, b2Vec2 position, std::shared_ptr<SceneNode> entity){
	//Apply movement

	b2Body* body = entity->getBody();
	
	body->SetTransform(position, 0);

    float vel = cos(angle)*muzzleVelocity;
    float impulseX = body->GetMass() * vel; //disregard time factor
    
    vel = sin(angle)*muzzleVelocity;
    float impulseY = body->GetMass() * vel; //disregard time factor
    body->ApplyLinearImpulse( b2Vec2(impulseX, impulseY), body->GetWorldCenter() );
}