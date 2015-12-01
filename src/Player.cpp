#include "Player.hpp"
#include "BananaGun.hpp"
#include "Banana.hpp"
#include "MissileLauncher.hpp"
#include "Missile.hpp"
#include <iostream>

 Player::Player(Game* game, int opponent){

 	mOpponent = opponent;
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
	polygonShape.SetAsBox(0.1,0.5);
	mFixtureDef.shape = &polygonShape;
	mFixtureDef.density = 1;
	mFixtureDef.friction = 0.1f;
	mFixtureDef.filter.categoryBits = PLAYER; //I am a PLAYER
    mFixtureDef.filter.maskBits = ~PLAYER; // I collide with everyhing else but another player. 
	mBody->CreateFixture(&mFixtureDef);

	// Declare and load a texture
	mTexture.loadFromFile("Astronaut-1.png");
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Initialize weapons: without players there can't be weapons.
	mWeapons.push_back(std::make_shared<BananaGun>(mGame));
	mWeapons.push_back(std::make_shared<MissileLauncher>(mGame, mOpponent));

	//create the aim dot
	aimDotTexture.loadFromFile("punapiste.png");
	aimDotSprite.setTexture(aimDotTexture);
	bounds = aimDotSprite.getLocalBounds();
	aimDotSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//create the jetpack flame
	jetpackTexture.loadFromFile("jetpack_flame.png");
	jetpackSprite.setTexture(jetpackTexture);
	bounds = jetpackSprite.getLocalBounds();
	jetpackSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//create the bazooka
	bazookaTexture.loadFromFile("bazooka.png");
	bazookaSprite.setTexture(bazookaTexture);
	bounds = bazookaSprite.getLocalBounds();
	bazookaSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	bazookaSprite.scale({-1,1});

	//Add foot sensor fixture: Used for determining on ground condition.
	polygonShape.SetAsBox(0.01f, 0.02f, b2Vec2(0,0.5f), 0);
	mFixtureDef.isSensor = true;
	b2Fixture* footSensorFixture = mBody->CreateFixture(&mFixtureDef);
	footSensorFixture->SetUserData((void*)PLAYER_FOOT_SENSOR_FIXTURE); //user data contains an identification number for the foot sensor, can be any number.




	//För helvete, varför är det absolut skit koda
	/*
	healthBar.setSize(sf::Vector2f(hp/100,1));
	healthBar.setOutlineColor(sf::Color::Red);
	healthBar.setOutlineThickness(5);
	healthBar.setPosition(10,20);
	*/

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
	//Checking for the moving direction
	if(previousXVelocity*x < 0) {//if the signs differ eg. direction changes. Note: this applies only to user movements
		direction *= -1;
		mSprite.scale({ -1, 1 });
	}

	//Apply some impulse to the body
	b2Vec2 vel = mBody->GetLinearVelocity();

	previousXVelocity = x;
    float velChange =10*x - vel.x;
    float impulseX = mBody->GetMass() * velChange; //disregard time factor
    mBody->ApplyLinearImpulse( b2Vec2(impulseX, 0), mBody->GetWorldCenter() );

    //Also change the position.
    float currentX = mBody->GetPosition().x;
    float currentY = mBody->GetPosition().y;
    mBody->SetTransform(b2Vec2(currentX+x, currentY), 0);
  
}

void Player::jump() {
	if(numGroundContacts > 0){
		jumpTimer = 20;
		b2Vec2 vel = mBody->GetLinearVelocity();
	    float velChange = PLAYER_JUMP_SPEED - vel.y;
	    float impulseY = mBody->GetMass() * velChange; //disregard time factor
	    mBody->ApplyLinearImpulse( b2Vec2(0, impulseY), mBody->GetWorldCenter() );
	}
	else if(jetpackFuel > 0 && jetpackReady && jumpTimer < 0) {
		//this code block creates a 0.33s delay between jump and jetpack fire.
		jetpackTimer = 10;
	    mBody->ApplyLinearImpulse( b2Vec2(0, JETPACK_THRUST), mBody->GetWorldCenter() );
	    jetpackFuel -= JETPACK_FUEL_CONSUMPTION;
	    //check if the fuel ran out
	    if(jetpackFuel < 0){
	    	jetpackReady = false;
	    }
	    std::cout << "Jetpack fuel remaining: " << jetpackFuel << std::endl;
	    //limiting the jetpack speed
	    b2Vec2 vel = mBody->GetLinearVelocity();
	    if(vel.y < -1*MAX_JETPACK_SPEED){
	    	mBody->SetLinearVelocity(b2Vec2(vel.x, -1*MAX_JETPACK_SPEED));
	    }

	}
}

void Player::fire() {
	float x = mBody->GetPosition().x+direction*sin(shootAngle)*GUN_BARREL_LENGTH; 
	float y = mBody->GetPosition().y+cos(shootAngle)*GUN_BARREL_LENGTH; 
	mWeapons[currentWeapon]->shoot(direction*shootAngle, b2Vec2(x,y), mBody->GetLinearVelocity(), mGame);
}

void Player::update(sf::Time deltaTime) {
	//position the aim dot
	float x = (mBody->GetPosition().x+direction*sin(shootAngle)*GUN_BARREL_LENGTH)*PIXELS_PER_METER; 
	float y = (mBody->GetPosition().y+cos(shootAngle)*GUN_BARREL_LENGTH)*PIXELS_PER_METER;
	aimDotSprite.setPosition(x,y);

	//position the possible jetpack flame
	x = (mBody->GetPosition().x-direction*0)*PIXELS_PER_METER; 
	y = (mBody->GetPosition().y+1.2)*PIXELS_PER_METER;
	jetpackSprite.setPosition(x,y);

	//position the bazooka
	x = (mBody->GetPosition().x+direction*sin(shootAngle)*0.2f*GUN_BARREL_LENGTH)*PIXELS_PER_METER; 
	y = (mBody->GetPosition().y+cos(shootAngle)*0.2f*GUN_BARREL_LENGTH)*PIXELS_PER_METER;

	bazookaSprite.setPosition(x,y);
	bazookaSprite.setRotation(-direction*shootAngle*RAD_TO_DEG);
	if(direction*gunDirection == -1){//checking for if the player has changed direction
		gunDirection = direction;
		bazookaSprite.scale({-1, 1});
	}
	

	//update jetpack status
	jetpackFuel += JETPACK_FUEL_FILL;
	if(jetpackFuel > JETPACK_MAX_FUEL){
		jetpackFuel = JETPACK_MAX_FUEL;
	}
	if(jetpackFuel > MIN_JETPACK_RELOAD_FUEL) {
		jetpackReady = true;
	}
	jetpackTimer--; //decrementing the jetpack timer.
	jumpTimer--;
	(void) deltaTime;
}

void Player::startContact(int id, Entity* contact) {
	
}

sf::Vector2f Player::getAimDotPosition() {
	return aimDotSprite.getPosition();
}

void Player::updateGroundContacts(int val) {
	numGroundContacts += val;
}

sf::Vector2f Player::returnPosition() {
	return mSprite.getPosition();
}

int Player::getType() {
	return PLAYER;
}

void Player::drawPlayer(sf::RenderTarget& target) {
	target.draw(aimDotSprite);
	if(jetpackTimer > 0 ){
		target.draw(jetpackSprite);
	}

	//Synchronize sprite coordinates with body
	b2Vec2 pos = mBody->GetPosition();
	spriteAngle = mBody->GetAngle() * RAD_TO_DEG;
	spritePosition.x = pos.x * PIXELS_PER_METER;
	spritePosition.y = pos.y * PIXELS_PER_METER;

	//Draw Sprite
	mSprite.setPosition(spritePosition);
	mSprite.setRotation(spriteAngle);
	target.draw(mSprite);

	//draw bazooka
	target.draw(bazookaSprite);
}

int Player::getHp() {
	return hp;
}

void Player::updateHp(int val) {
	hp += val;
	std::cout << "Hp changed: " << val << std::endl;
}

void Player::scrollWeapons() {
	currentWeapon++;
	std::cout << "changed weapon" << std::endl;
	if(currentWeapon >= mWeapons.size()) {
		currentWeapon = 0;
	}
}