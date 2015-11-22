#ifndef ENTITY_HH
#define ENTITY_HH

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

/*
Entity is a base class for all moveable objects players, projectiles, powerups, etc.
*/

#include "SceneNode.hpp"
class Game;

class Entity : public SceneNode{

public:

	Entity(){}
	~Entity(){}

	void draw(sf::RenderTarget& target);
	b2Body* getBody() const;
	sf::Sprite getSprite();

	bool isAlive();
	void setAlive(bool status);

	virtual void update(sf::Time deltaTime) = 0;
	virtual void startContact() = 0;


protected:
	//Sprite
	sf::Sprite mSprite;
	sf::Texture mTexture;
	sf::Vector2f spritePosition;
	float spriteAngle = 0;

	//possible aim dot (players only)
	sf::Texture aimDotTexture;
	sf::Sprite aimDotSprite;

	//Body
	b2Body* mBody;
	b2BodyDef mBodyDef;

	//Fixture
	b2PolygonShape mFixture;
	b2FixtureDef mFixtureDef;

	//The Box2D world
	b2World* mEntityWorld;

	//The game
	Game* mGame;

	//Additional information
	bool alive = false;
	float lifeTime = 0; 
};

#endif