#include "Rifle.hpp"
#include "Bullet.hpp"
#include <iostream>

Rifle::Rifle(Game* game): Weapon(30, 0.1f, 4, 25.0f, "texture/ak47.png", game){
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

	std::shared_ptr<Bullet> b = std::make_shared<Bullet>(mGame);
	mGame->getSceneNode()->attachChild(std::dynamic_pointer_cast<SceneNode>(b));
	launchProjectile(angle, position, preSpeed, b);

	fireClock.restart();//restarting the fire rate observing clock
	ammo--;//reducing ammo in the clip by 1.
}