#include "MissileLauncher.hpp"
#include "Missile.hpp"
#include <iostream>

MissileLauncher::MissileLauncher(Game* game, int target): Weapon(10, 1, 3, 20.0f, game){
	//Adding a missile to the stack

	for(int i = 0; i < 10; i++){
		std::shared_ptr<Missile> m = std::make_shared<Missile>(game, target);
		game->getSceneNode()->attachChild(std::static_pointer_cast<SceneNode>(m));
	}
	ammo = 10;
}

void MissileLauncher::shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game){

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

	auto entities = mGame->getSceneNode()->getChildren();
	for(auto entity: entities){
		if(!entity->isAlive() && typeid(*entity) == typeid(Missile)) {
			entity->setAlive(true);
			entity->getBody()->GetFixtureList()[0].SetSensor(false); //missiles have only 1 fixture
			launchProjectile(angle, position, preSpeed, entity);
			fireClock.restart();//restarting the fire rate observing clock
			ammo--;
			break;
		}
	}
}