#include "BananaGun.hpp"
#include "Banana.hpp"
#include <iostream>

BananaGun::BananaGun(Game* game): Weapon(10, 0.5f, 3, 10.0f, game){
	//Adding bananas to the stack
	
	for(unsigned int i = 0; i<3*clipSize; i++){
		std::shared_ptr<Banana> b = std::make_shared<Banana>(game);
		game->getSceneNode()->attachChild(std::static_pointer_cast<SceneNode>(b));
	}
	std::cout << "Added " << 3*clipSize << " Bananas to entities by BananaGun constructor." << std::endl;
	ammo = clipSize;
}

void BananaGun::shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game){
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
		if(!entity->isAlive() && typeid(*entity) == typeid(Banana)) {
			entity->setAlive(true);
			entity->getBody()->SetAngularVelocity(1);//Adding a nice spin
			launchProjectile(angle, position, preSpeed, entity);
			fireClock.restart();//restarting the fire rate observing clock
			ammo--;//reducing ammo in the clip by 1.
			break;
		}
	}
}