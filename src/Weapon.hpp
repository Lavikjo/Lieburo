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
	virtual void shoot(float, b2Vec2, b2Vec2, Game*) = 0;
	void launchProjectile(float angle, b2Vec2 position, b2Vec2 preSpeed, std::shared_ptr<SceneNode> entity);

	unsigned int getAmmo() const;
	unsigned int getClipSize() const;
protected:
	unsigned int clipSize;
	unsigned int ammo;
	float fireRate; //Time interval of shots;
	float reloadTime;
	float muzzleVelocity;
	Game* mGame;
	b2World* mGameWorld;
	sf::Clock fireClock;

};

#endif