#include "Bullet.hpp"

Bullet::Bullet(Game* game){

	baseConstructor(game, "texture/bullet.png");

	//Projectiles aren't alive by default
	alive = false;
}

void Bullet::update(sf::Time deltaTime) {
	if(alive) {
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > MAX_LIFETIME) {
			alive = false;
		}
	}else {
		lifeTime = 0;
	}	
}

void Bullet::startContact(Entity* contact){
	if(typeid(*contact) == typeid(Player)) {
		if(lifeTime > 0.05) {
			contact->updateHp(-10);
		}
	}
	lifeTime = MAX_LIFETIME +1; //dies next

}

