#include "Rifle.hpp"
#include "Bullet.hpp"
#include <iostream>

Rifle::Rifle(Game* game): Weapon(30, 0.1f, 4, 25.0f, "ak47.png", game){
	//Adding bananas to the stack
	ammo = clipSize;
	(void) game;
}

void Rifle::shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game){
	(void) game;
	//Checking the fire rate
	if(fireClock.getElapsedTime().asSeconds() < fireRate) {
		return;
	}
	//Handling emptied clip
	if(ammo == 0) {
		if(fireClock.getElapsedTime().asSeconds() > reloadTime) {
			ammo = clipSize; //reload time up -> load clip
		}
		else {
			return;
		}
	}

	game->getSceneNode()->attachChild(std::static_pointer_cast<SceneNode>(std::make_shared<Bullet>(mGame)));

	auto entities = mGame->getSceneNode()->getChildren();
	for(auto entity: entities){
		if(!entity->isAlive() && typeid(*entity) == typeid(Bullet)) {
			entity->setAlive(true);
			entity->getBody()->GetFixtureList()[0].SetSensor(false); //bananas have only 1 fixture
			launchProjectile(angle, position, preSpeed, entity);
			fireClock.restart();//restarting the fire rate observing clock
			ammo--;//reducing ammo in the clip by 1.
			break;
		}
	}
}