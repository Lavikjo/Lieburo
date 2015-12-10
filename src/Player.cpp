#include "Player.hpp"
#include "BananaGun.hpp"
#include "Banana.hpp"
#include "MissileLauncher.hpp"
#include "Missile.hpp"
#include "Rifle.hpp"
#include "Bullet.hpp"

#include "Shrapnel.hpp"

 Player::Player(Game* game, int opponent){

 	mOpponent = opponent;

 	mEntityWorld = game->getWorld();
 	mGame = game;
	//Create the dynamic body

	mBodyDef.type = b2_dynamicBody;
	mBodyDef.fixedRotation = true; // prevent rotation
	mBodyDef.angle = 0;
	mBodyDef.fixedRotation = true; // prevent rotation
	mBody = mEntityWorld->CreateBody(&mBodyDef);
	mBody->SetUserData(this);

	//respawn the player to a random position. 100px margin.
	mBody->SetTransform(b2Vec2((rand()%(GAMEFIELD_WIDTH-200)+100)/(int)PIXELS_PER_METER,
		(rand()%(GAMEFIELD_HEIGHT-200)+100)/(int)PIXELS_PER_METER), 0);

	b2Vec2 pos = mBody->GetPosition();
	std::cout <<"x: "<<pos.x*30<<", y: "<<pos.y*30<<std::endl;
	// Declare and load a texture
	mTexture.loadFromFile("texture/Astronaut-1.png");
	
	// Create a sprite
	mSprite.setTexture(mTexture);
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//Add a fixture to the body
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(0.5f*bounds.width / PIXELS_PER_METER, 0.5f*bounds.height/PIXELS_PER_METER);
	mFixtureDef.shape = &polygonShape;
	mFixtureDef.density = 1;
	mFixtureDef.friction = 0.1f;
	mFixtureDef.filter.categoryBits = PLAYER; //I am a PLAYER
    mFixtureDef.filter.maskBits = ~PLAYER; // I collide with everyhing else but another player. 
	mBody->CreateFixture(&mFixtureDef);

	//Initialize weapons: without players there can't be weapons.
	mWeapons.push_back(std::make_shared<BananaGun>(mGame));
	mWeapons.push_back(std::make_shared<MissileLauncher>(mGame, mOpponent));
	mWeapons.push_back(std::make_shared<Rifle>(mGame));

	//create the aim dot
	aimDotTexture.loadFromFile("texture/punapiste.png");
	aimDotSprite.setTexture(aimDotTexture);
	bounds = aimDotSprite.getLocalBounds();
	aimDotSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//create the jetpack flame
	jetpackTexture.loadFromFile("texture/jetpack_flame.png");
	jetpackSprite.setTexture(jetpackTexture);
	bounds = jetpackSprite.getLocalBounds();
	jetpackSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//create the weapon
	weaponTexture = mWeapons[currentWeapon]->getTexture();
	weaponSprite.setTexture(weaponTexture);
	bounds = weaponSprite.getLocalBounds();
	weaponSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	weaponSprite.scale({-1,1});

	//Add foot sensor fixture: Used for determining on ground condition.
	polygonShape.SetAsBox(bounds.width/PIXELS_PER_METER/5.0f, bounds.width/PIXELS_PER_METER/5.0f, b2Vec2(0,bounds.height/PIXELS_PER_METER), 0);
	mFixtureDef.isSensor = true;
	b2Fixture* footSensorFixture = mBody->CreateFixture(&mFixtureDef);
	footSensorFixture->SetUserData((void*)PLAYER_FOOT_SENSOR_FIXTURE); //user data contains an identification number for the foot sensor, can be any number.

	alive = true;	
	setCommands();	
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

	weaponSprite.setPosition(x,y);
	weaponSprite.setRotation(-direction*shootAngle*RAD_TO_DEG);
	if(direction*gunDirection == -1){//checking for if the player has changed direction
		gunDirection = direction;
		weaponSprite.scale({-1, 1});
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

	//The lifelost-handler

	//Update hp back up after player has pressed fire
	if(respawning){
		hp += 1;
		if(hp>0.3f*PLAYER_MAX_HP){
			waitingForRespawn = false;//Taking this flag off now to prevent a gunshot from the fire button press.
		}
		if(hp >= PLAYER_MAX_HP){
			hp = PLAYER_MAX_HP;
			respawning = false;
		}
	}

	//If the hp is gone
	else if(hp <= 0){
		hp = 0;
		//Single-time checks
		if(!waitingForRespawn){
			lives--;
			bloodToSpill = 70;
			//setting the body fixture to collide with nothing.
			b2Fixture* f = mBody->GetFixtureList()->GetNext();
			b2Filter filter;
			filter.maskBits = 0; 
			f->SetFilterData(filter);

			//Checking for game over
			if(lives == 0) {
				alive = false;
				mGame->gameOver(mOpponent);
				return;
			}
			std::cout<<"Press fire to respawn"<<std::endl;
		}
		waitingForRespawn = true;

		//waiting for user input to load hp and reposition.
		if(sf::Keyboard::isKeyPressed(keys[5])){
			respawning = true;
			respawn();
		}
	}

	//Spill blood
	spillBlood(bloodToSpill);
}

void Player::startContact(Entity* contact) {
	(void) contact;	
}

sf::Vector2f Player::getAimDotPosition(){
	return aimDotSprite.getPosition();
}

void Player::updateGroundContacts(int val) {
	numGroundContacts += val;
}

sf::Vector2f Player::getSpritePosition() {
	return mSprite.getPosition();
}


void Player::drawPlayer(sf::RenderTarget& target) {
	if(!waitingForRespawn){
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

		//draw weapon
		target.draw(weaponSprite);
	}
}

int Player::getHp() const{
	return hp;
}

float Player::getFuel() const{
	return jetpackFuel;
}

unsigned int Player::getCurrentAmmo() const {
	return mWeapons[currentWeapon]->getAmmo();
}

unsigned int Player::getCurrentClipSize() const {
	return mWeapons[currentWeapon]->getClipSize();
}

void Player::updateHp(int val) {
	hp += val;
	bloodToSpill = -val*2;
}

void Player::scrollWeapons() {
	currentWeapon++;
	if(currentWeapon >= mWeapons.size()) {
		currentWeapon = 0;
	}

	weaponTexture = mWeapons[currentWeapon]->getTexture();
	weaponSprite.setTexture(weaponTexture, true);
	sf::FloatRect bounds = weaponSprite.getLocalBounds();
	weaponSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Player::setCommands() {
	std::ifstream buttonsFile("buttons.txt");
	std::string line;	
	unsigned int numKeys = mGame->getOptions()->getNumberPlayerKeys();

	if (mOpponent == 2) {
		for (unsigned int i = 0; i < numKeys; i++) {
			std::getline(buttonsFile, line);
			std::stringstream iss(line);
			std::string s = iss.str();

			keys.push_back(mGame->getOptions()->getButtons()[s]);
			keyNames.push_back(s);
		}
		buttonsFile.close();
	}

	else if (mOpponent == 1) {
		for (unsigned int i = 0; i < (numKeys * 2); i++) {
			std::getline(buttonsFile, line);
			if (i < 7) {
				continue;
			}
			std::stringstream iss(line);
			std::string s = iss.str();

			keys.push_back(mGame->getOptions()->getButtons()[s]);
			keyNames.push_back(s);
		}
		buttonsFile.close();
	}
}

void Player::resetCommands() {
	std::ifstream buttonsResetFile("buttonsReset.txt");
	std::string line;
	std::ofstream newButtonsFile("newButtons.txt");
	unsigned int numKeys = mGame->getOptions()->getNumberPlayerKeys();

	if (mOpponent == 2) {
		for (unsigned int i = 0; i < numKeys * 2; i++) {
			std::getline(buttonsResetFile, line);
			newButtonsFile << line << std::endl;
			if (i < numKeys) {
				keys[i] = mGame->getOptions()->getButtons()[line];
				keyNames[i] = line;
			}
		}
	}
	else if (mOpponent == 1) {
		for (unsigned int i = 0; i < numKeys * 2; i++) {
			std::getline(buttonsResetFile, line);
			newButtonsFile << line << std::endl;
			if (i >= numKeys) {
				keys[i - numKeys] = mGame->getOptions()->getButtons()[line];
				keyNames[i - numKeys] = line;
			}
		}
	}
	if (rename("newButtons.txt", "buttons.txt")) {
		std::cout << "Error renaming file." << std::endl;
	}
	buttonsResetFile.close();
}



void Player::respawn(){

	//setting the body fixture to collide with nothing.
	b2Fixture* f = mBody->GetFixtureList()->GetNext();
	b2Filter filter;
	filter.maskBits = ~PLAYER; 
	filter.categoryBits = PLAYER; //I am a PLAYER
	f->SetFilterData(filter);

	//respawn the player to a random position. 100px margin.
	mBody->SetTransform(b2Vec2((rand()%(GAMEFIELD_WIDTH-200)+100)/(int)PIXELS_PER_METER,
		(rand()%(GAMEFIELD_HEIGHT-200)+100)/(int)PIXELS_PER_METER), 0);
	jetpackFuel = JETPACK_MAX_FUEL;
} 

void Player::handleUserInput(){
	if(!waitingForRespawn){
		if (sf::Keyboard::isKeyPressed(keys[0])) {
		    jump();
		}
	    if (sf::Keyboard::isKeyPressed(keys[1])) {
		    movePlayerX(-0.1f);
		}
		if (sf::Keyboard::isKeyPressed(keys[2])) {
		    movePlayerX(0.1f);
		}
		if (sf::Keyboard::isKeyPressed(keys[3])) {
		    aim(3);
		}
		if (sf::Keyboard::isKeyPressed(keys[4])) {
		    aim(-3);
		}
		if (sf::Keyboard::isKeyPressed(keys[5])) {
		    fire();
		}
	}
}

void Player::spillBlood(int amount){
	for(auto i = 0; i < amount; i++) {
		//Create blood shrapnel
		std::shared_ptr<Shrapnel> s = std::make_shared<Shrapnel>(mGame, "texture/blood.png", 2.0f, 0, 0);		
  		
  		//Setting the blood not to spin
  		b2Body* body = s->getBody();
  		body->SetFixedRotation(true);

  		//setting the body fixture to collide with nothing.
		b2Fixture* f = body->GetFixtureList();
		b2Filter filter;
		filter.maskBits = BOUNDARY; //I collide with boundary only 
		filter.categoryBits = BLOOD;
		f->SetFilterData(filter);
		
		mGame->getSceneNode()->attachChild(std::dynamic_pointer_cast<SceneNode>(s));

		//Make it fly
		s->setAlive(true);

		float a = (rand()%100)/100.0f;

		float angle = a*i; //The direction is random enough as i is radians.

		float x = mBody->GetPosition().x+direction*sin(angle+a)*0.5f*a*GUN_BARREL_LENGTH; 
		float y = mBody->GetPosition().y+cos(angle+a)*0.5f*a*GUN_BARREL_LENGTH;
		body->SetTransform(b2Vec2(x,y), 0);

		float velX = sin(angle)*5.0f*a;
		float velY = cos(angle)*5.0f*a;

		body->SetLinearVelocity(b2Vec2(velX, velY));
	}
	bloodToSpill = 0;
}

bool Player::isWaitingForRespawn() const{
	return waitingForRespawn;
}

std::vector<sf::Keyboard::Key>& Player::getKeys(){
	return keys;
}

std::vector<std::string>& Player::getKeyNames(){
	return keyNames;
}