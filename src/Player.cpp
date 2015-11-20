#include "Player.hpp"
#include "BananaGun.hpp"
#include <iostream>

 Player::Player(Game* game){

 	mEntityWorld = game->getWorld();
 	mGame = game;
	//Create the dynamic body

	mBodyDef.type = b2_dynamicBody;
	mBodyDef.fixedRotation = true; // prevent rotation
	mBodyDef.position.Set(20, 10);
	mBodyDef.angle = 0;
	mBodyDef.fixedRotation = true; // prevent rotation
	mBody = mEntityWorld->CreateBody(&mBodyDef);
	mBody->SetUserData(this);

	//Add a fixture to the body
	b2PolygonShape boxShape;
	boxShape.SetAsBox(10,30);
	mFixtureDef.shape = &boxShape;
	mFixtureDef.density = 20;
	mBody->CreateFixture(&mFixtureDef);

	// Declare and load a texture
	mTexture.loadFromFile("Astronaut-1.png");
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	mSprite.setPosition(100, 25);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Initialize weapons: without players ther can't be weapons.
	mWeapons.push_back(std::make_shared<BananaGun>(mGame));

	alive = true;
	
}

Player::~Player(){
	mEntityWorld->DestroyBody(mBody);
}

void Player::fire() {
	//std::shared_ptr<BananaGun> w = std::static_pointer_cast<BananaGun>(mWeapons[currentWeapon]);
	mWeapons[currentWeapon]->shoot(shootAngle, mBody->GetPosition(),mGame);
}

void Player::movePlayer(float x, float y) {
	//Apply movement
	b2Vec2 vel = mBody->GetLinearVelocity();
    
    float velChange = x - vel.x;
    float impulseX = mBody->GetMass() * velChange; //disregard time factor
    
    velChange = y - vel.y;
    float impulseY = mBody->GetMass() * velChange; //disregard time factor
    mBody->ApplyLinearImpulse( b2Vec2(impulseX, impulseY), mBody->GetWorldCenter() );
}


void Player::update(sf::Time deltaTime){
	(void) deltaTime;
}

void Player::startContact(){

}

