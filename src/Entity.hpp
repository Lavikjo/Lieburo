#ifndef ENTITY_HH
#define ENTITY_HH

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

/*
Entity is a base class for all moveable objects players, projectiles, powerups, etc.
*/

#include "SceneNode.hpp"
class Game;

class Entity : public SceneNode {

public:

	Entity(){}
	~Entity(){}

	void draw(sf::RenderTarget& target);
	b2Body* getBody() const;
	sf::Sprite getSprite() const;

	bool isAlive();
	void setAlive(bool status);

	void baseConstructor(Game*, std::string);

	virtual void update(sf::Time) = 0;
	virtual void startContact(Entity*){}

	virtual void drawPlayer(sf::RenderTarget&){}
	virtual void updateHp(int){}


protected:
	//Sprite
	sf::Sprite mSprite;
	sf::Texture mTexture;
	sf::Vector2f spritePosition;
	float spriteAngle = 0;

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

	int type = 0;


};

#endif