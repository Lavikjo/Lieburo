#include "MissileLauncher.hpp"
#include "Missile.hpp"
#include <iostream>

MissileLauncher::MissileLauncher(Game* game, int target): Weapon(10, 1, 3, 30.0f, "texture/bazooka.png", game){
	//Adding a missile to the stack
	mTarget = target;
	
	ammo = 10;

	(void) game;
}

void MissileLauncher::shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game){

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
	std::shared_ptr<Missile> m = std::make_shared<Missile>(mGame, mTarget);
	mGame->getSceneNode()->attachChild(std::dynamic_pointer_cast<SceneNode>(m));
	launchProjectile(angle, position, preSpeed, m);

	fireClock.restart();//restarting the fire rate observing clock
	ammo--;//reducing ammo in the clip by 1.
}