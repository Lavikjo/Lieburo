#include "GravityInverter.hpp"

/*
GravityInverter::GravityInverter(Game* game, std::string textureName, bool bodyTypeDynamic, float gravValue){
	alive = true;
	mEntityWorld = game->getWorld(); 
	mGame = game;
	//Create the dynamic body
	if(bodyTypeDynamic)
		mBodyDef.type = b2_dynamicBody;
	else
		mBodyDef.type = b2_staticBody;
	mBodyDef.position.Set((rand()/(int)PIXELS_PER_METER)%(GAMEFIELD_WIDTH/(int)PIXELS_PER_METER),
	(rand()/(int)PIXELS_PER_METER)%(GAMEFIELD_HEIGHT/(int)PIXELS_PER_METER));
	mBodyDef.angle = 0;
	mBody = mEntityWorld->CreateBody(&mBodyDef);
	mBody->SetUserData(this);
	
	// Declare and load a texture
	mTexture.loadFromFile(textureName);
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Add a fixture to the body
	b2PolygonShape boxShape;
	boxShape.SetAsBox(0.5f*bounds.width / PIXELS_PER_METER, 0.5f*bounds.height/PIXELS_PER_METER);
	mFixtureDef.shape = &boxShape;
	mFixtureDef.density = 1;

	mBody->CreateFixture(&mFixtureDef);
}
*/
void GravityInverter::startContact(Entity* contact){
	if(typeid(*contact) == typeid(Player)){
		if(alive){
			changeGravity(gravityValue);
			mSprite.setColor(sf::Color(255,255,255,0));
			setActive(true);
		}
	}
}

void GravityInverter::update(sf::Time deltaTime) {
	if(alive && isActive()){
		lifeTime += deltaTime.asSeconds();	
		if(lifeTime > LIFETIME){
			changeGravity(9.8f);
			setActive(false);
			mEntityWorld->DestroyBody(mBody);
		}
	}
		else
			lifeTime = 0;
}


void GravityInverter::changeGravity(float gravityValue){
	mEntityWorld->SetGravity(b2Vec2(0, gravityValue));
}