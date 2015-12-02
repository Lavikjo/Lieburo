#include "Bullet.hpp"

Bullet::Bullet(Game* game) : Projectile(){

	baseConstructor(game, "bullet.png");

	//Projectiles aren't alive by default
	alive = false;
}

void Bullet::update(sf::Time deltaTime) {
	if(alive) {
		lifeTime += deltaTime.asSeconds();
		if(lifeTime > MAX_LIFETIME) {
			alive = false;
			this->getBody()->GetFixtureList()[0].SetSensor(true);
		}
	}else {
		lifeTime = 0;
	}	
}

void Bullet::startContact(int id, Entity* contact){
	if(typeid(*contact) == typeid(Player)) {
		if(alive) {
			contact->updateHp(-5);
		}
	}

	(void) id;
}

int Bullet::getType() {
	return 0;
}