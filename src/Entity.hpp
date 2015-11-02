#ifndef ENTITY_HH
#define ENTITY_HH

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

/*
Entity is an abstract base class for all moveable objects players, projectiles, powerups, etc.
*/

class Entity {

public:
	
	Entity() = 0;
	~Entity() = 0;

	//sprite
	sf::Sprite sprite;
	sf::Texture texture

	//rigid body
	b2Body* body;
	b2BodyDef body_def;

	//fixture
	b2PolygonShape fixture;
	b2FixtureDef fixture_def;

	//position vector
	sf::Vector2f position;
};

#endif