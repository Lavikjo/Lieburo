#include "Weapon.hpp"

Weapon::Weapon(unsigned int clipSize, float fireRate, float reloadTime, float muzzleVelocity, std::string texture, Game* game)
	:clipSize(clipSize), fireRate(fireRate), reloadTime(reloadTime), muzzleVelocity(muzzleVelocity), mGame(game), mGameWorld(game->getWorld()){
		ammo = clipSize;
		
		//load the texture
		mWeaponTexture.loadFromFile(texture);
	}

void Weapon::launchProjectile(float angle, b2Vec2 position, b2Vec2 preSpeed, std::shared_ptr<SceneNode> entity){
	//Apply movement and set the entity alive

	entity->setAlive(true);

	b2Body* body = entity->getBody();
	
	body->SetTransform(position, 0);

    float velX = sin(angle)*muzzleVelocity + preSpeed.x;
    
    float velY = cos(angle)*muzzleVelocity + preSpeed.y;
    

    body->SetLinearVelocity(b2Vec2(velX, velY));

}

unsigned int Weapon::getAmmo(){
	if(ammo == 0 && fireClock.getElapsedTime().asSeconds() > reloadTime) {
		ammo = clipSize; //reload time up -> load clip
	}
	return ammo;
}

unsigned int Weapon::getClipSize() const {
	return clipSize;
}

sf::Texture Weapon::getTexture() const{
	return mWeaponTexture;
}