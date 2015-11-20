#include "BananaGun.hpp"
#include "Banana.hpp"
#include <iostream>

BananaGun::BananaGun(Game* game): Weapon(10, 5, 1, 10, game){
	//Adding bananas to the stack
	
	for(unsigned int i = 0; i<3*clipSize; i++){
		std::shared_ptr<Banana> b = std::make_shared<Banana>(game);
		game->getSceneNode()->attachChild(std::static_pointer_cast<SceneNode>(b));
	}
	std::cout << "Added " << 3*clipSize << " Bananas to entities by BananaGun constructor." << std::endl;
}

void BananaGun::shoot(float angle, b2Vec2 position, Game* game){
	auto entities = mGame->getSceneNode()->getChildren();
	for(auto entity: entities){
		if(!entity->isAlive() && typeid(*entity) == typeid(Banana)) {
			entity->setAlive(true);
			entity->getBody()->ApplyTorque(200);//Adding a nice spin
			launchProjectile(angle, position, entity);
			break;
		}
	}
}