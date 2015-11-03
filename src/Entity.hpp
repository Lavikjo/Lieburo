#ifndef ENTITY_HH
#define ENTITY_HH

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

/*
Entity is an abstract base class for all moveable objects players, projectiles, powerups, etc.
*/

class Entity {

public:

	//sprite
	sf::Sprite sprite;
	sf::Texture texture

	//rigid body
	b2Body* body;
	b2BodyDef body_def;

	//fixture
	b2PolygonShape fixture;
	b2FixtureDef fixture_def;

	
private:
	sf::Vector2f velocity;
};

#endif