#include "BananaGun.hpp"
#include "Banana.hpp"
#include <iostream>

BananaGun::BananaGun(Game* game): Weapon(4, 0.5f, 5, 25.0f, "texture/bananagun.png", game){
	
	ammo = clipSize;
	(void) game;
}

void BananaGun::shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game){
	(void) game;
	//Checking the fire rate
	if(fireClock.getElapsedTime().asSeconds() < fireRate) {
		return;
	}
	//Checking for empty clip.
	if(ammo == 0) {
		if(fireClock.getElapsedTime().asSeconds() > reloadTime) {
			ammo = clipSize; //reload time up -> load clip
		}
		else {
			return;
		}
	}
	std::shared_ptr<Banana> b = std::make_shared<Banana>(mGame);
	mGame->getSceneNode()->attachChild(std::dynamic_pointer_cast<SceneNode>(b));
	b->getBody()->SetAngularVelocity(25);//Adding a nice spin
	launchProjectile(angle, position, preSpeed, b);

	fireClock.restart();//restarting the fire rate observing clock
	ammo--;//reducing ammo in the clip by 1.
}