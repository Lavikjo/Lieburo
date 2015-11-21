#ifndef WEAPON_HH
#define WEAPON_HH

#include <Box2D/Box2D.h>
#include <SFML/System.hpp>
#include <iostream>

#include "Game.hpp"
#include "SceneNode.hpp"


class Weapon {

public:
	Weapon(unsigned int clipSize, float fireRate, float reloadTime, float muzzleVelocity, Game* game);
	~Weapon(){}
	virtual void shoot(float angle, b2Vec2 position, b2Vec2 preSpeed, Game* game) = 0;
	void launchProjectile(float angle, b2Vec2 position, b2Vec2 preSpeed, std::shared_ptr<SceneNode> entity);

protected:
	unsigned int clipSize;
	unsigned int ammo;
	float fireRate; //Time interval of shots;
	float reloadTime;
	Game* mGame;
	b2World* mGameWorld;
	sf::Clock fireClock;

private:
	float muzzleVelocity;
};

#endif