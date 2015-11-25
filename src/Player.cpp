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
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(1,2);
	mFixtureDef.shape = &polygonShape;
	mFixtureDef.density = 2;
	mFixtureDef.friction = 1.0f;
	mBody->CreateFixture(&mFixtureDef);

	// Declare and load a texture
	mTexture.loadFromFile("Astronaut-1.png");
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	mSprite.setPosition(100, 25);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Initialize weapons: without players there can't be weapons.
	mWeapons.push_back(std::make_shared<BananaGun>(mGame));

	//create the aim dot
	aimDotTexture.loadFromFile("punapiste.png");
	aimDotSprite.setTexture(aimDotTexture);
	bounds = aimDotSprite.getLocalBounds();
	aimDotSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Add foot sensor fixture: Used for determining on ground condition.
	polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0,-1), 0);
	mFixtureDef.isSensor = true;
	b2Fixture* footSensorFixture = mBody->CreateFixture(&mFixtureDef);
	footSensorFixture->SetUserData((void*) 3); //user data contains an identification number for the foot sensor, can be any number.

	alive = true;
	
}

Player::~Player(){
	mEntityWorld->DestroyBody(mBody);
}

void Player::aim(float angleChange) {
	shootAngle += angleChange*DEG_TO_RAD;
	if(shootAngle > MAX_SHOOT_ANGLE) {
		shootAngle = MAX_SHOOT_ANGLE;
	} else if(shootAngle < MIN_SHOOT_ANGLE) {
		shootAngle = MIN_SHOOT_ANGLE;
	}
}

void Player::movePlayerX(float x) {
	//Apply movement
	b2Vec2 vel = mBody->GetLinearVelocity();

	//Separate handlers for on ground and in air

	//Checking for the moving direction
	if(previousXVelocity*x < 0) {//if the signs differ eg. direction changes. Note: this applies only to user movements
		direction *= -1;
		mSprite.scale({ -1, 1 });
	}

	previousXVelocity = x;
    

    float velChange = x - vel.x;
    float impulseX = mBody->GetMass() * velChange; //disregard time factor
    
    mBody->ApplyLinearImpulse( b2Vec2(impulseX, 0), mBody->GetWorldCenter() );
}

void Player::movePlayerY(float y) {
	//Apply movement
	b2Vec2 vel = mBody->GetLinearVelocity();

    float velChange = y - vel.y;
    float impulseY = mBody->GetMass() * velChange; //disregard time factor
    mBody->ApplyLinearImpulse( b2Vec2(0, impulseY), mBody->GetWorldCenter() );
}

void Player::fire() {
	float x = mBody->GetPosition().x+direction*sin(shootAngle)*GUN_BARREL_LENGTH; 
	float y = mBody->GetPosition().y+cos(shootAngle)*GUN_BARREL_LENGTH; 
	mWeapons[currentWeapon]->shoot(direction*shootAngle, b2Vec2(x,y), mBody->GetLinearVelocity(), mGame);
}

void Player::update(sf::Time deltaTime) {
	//draw the aim dot
	float x = (mBody->GetPosition().x+direction*sin(shootAngle)*GUN_BARREL_LENGTH)*PIXELS_PER_METER; 
	float y = (mBody->GetPosition().y+cos(shootAngle)*GUN_BARREL_LENGTH)*PIXELS_PER_METER;
	aimDotSprite.setPosition(x,y);
	(void) deltaTime;
}

void Player::startContact() {

}

sf::Vector2f Player::getAimDotPosition() {
	return aimDotSprite.getPosition();
}

void Player::addGorundContact() {
	numGroundContacts++;
}

void Player::removeGroundContact() {
	numGroundContacts--;
}

sf::Vector2f Player::returnPosition() {
	return mSprite.getPosition();
}