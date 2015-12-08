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

	//respawn the player to a random position.
	mBody->SetTransform(b2Vec2((rand()/(int)PIXELS_PER_METER)%(GAMEFIELD_WIDTH/(int)PIXELS_PER_METER),
		(rand()/(int)PIXELS_PER_METER)%(GAMEFIELD_HEIGHT/(int)PIXELS_PER_METER)), 0);

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

	setButtons();
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

sf::Vector2f Player::getAimDotPosition() const {
	return aimDotSprite.getPosition();
}

void Player::updateGroundContacts(int val) {
	numGroundContacts += val;
}

sf::Vector2f Player::returnPosition() {
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
	std::ifstream buttons("buttons.txt");
	std::string line;	
	if (mOpponent == 2) {
		for (unsigned int i = 0; i < PLAYER_KEYS; i++) {
			std::getline(buttons, line);
			std::stringstream iss(line);
			std::string s = iss.str();

			keys.push_back(button[s]);
			keyNames.push_back(s);
		}
		buttons.close();
	}

	else if (mOpponent == 1) {
		for (unsigned int i = 0; i < (PLAYER_KEYS * 2); i++) {
			std::getline(buttons, line);
			if (i < 7) {
				continue;
			}
			std::stringstream iss(line);
			std::string s = iss.str();

			keys.push_back(button[s]);
			keyNames.push_back(s);
		}
	}
}

void Player::resetCommands() {
	std::ifstream buttonsReset("buttonsReset.txt");
	std::string line;
	std::ofstream newButtons("newButtons.txt");
	if (mOpponent == 2) {
		for (unsigned int i = 0; i < PLAYER_KEYS * 2; i++) {
			std::getline(buttonsReset, line);
			newButtons << line << std::endl;
			if (i < PLAYER_KEYS) {
				keys[i] = button[line];
				keyNames[i] = line;
			}
		}
	}
	else if (mOpponent == 1) {
		for (unsigned int i = 0; i < PLAYER_KEYS * 2; i++) {
			std::getline(buttonsReset, line);
			newButtons << line << std::endl;
			if (i >= PLAYER_KEYS) {
				keys[i - PLAYER_KEYS] = button[line];
				keyNames[i - PLAYER_KEYS] = line;
			}
		}
	}
	if (rename("newButtons.txt", "buttons.txt")) {
		std::cout << "Error renaming file." << std::endl;
	}
	buttonsReset.close();
}

void Player::setButtons() {
	button.insert(std::make_pair("A", sf::Keyboard::A));
	button.insert(std::make_pair("B", sf::Keyboard::B));
	button.insert(std::make_pair("C", sf::Keyboard::C));
	button.insert(std::make_pair("D", sf::Keyboard::D));
	button.insert(std::make_pair("E", sf::Keyboard::E));
	button.insert(std::make_pair("F", sf::Keyboard::F));
	button.insert(std::make_pair("G", sf::Keyboard::G));
	button.insert(std::make_pair("H", sf::Keyboard::H));
	button.insert(std::make_pair("I", sf::Keyboard::I));
	button.insert(std::make_pair("J", sf::Keyboard::J));
	button.insert(std::make_pair("K", sf::Keyboard::K));
	button.insert(std::make_pair("L", sf::Keyboard::L));
	button.insert(std::make_pair("M", sf::Keyboard::M));
	button.insert(std::make_pair("N", sf::Keyboard::N));
	button.insert(std::make_pair("O", sf::Keyboard::O));
	button.insert(std::make_pair("Q", sf::Keyboard::Q));
	button.insert(std::make_pair("R", sf::Keyboard::R));
	button.insert(std::make_pair("S", sf::Keyboard::S));
	button.insert(std::make_pair("T", sf::Keyboard::T));
	button.insert(std::make_pair("U", sf::Keyboard::U));
	button.insert(std::make_pair("W", sf::Keyboard::W));
	button.insert(std::make_pair("X", sf::Keyboard::X));
	button.insert(std::make_pair("Y", sf::Keyboard::Y));
	button.insert(std::make_pair("Z", sf::Keyboard::Z));
	button.insert(std::make_pair("Num0", sf::Keyboard::Num0));
	button.insert(std::make_pair("Num1", sf::Keyboard::Num1));
	button.insert(std::make_pair("Num2", sf::Keyboard::Num2));
	button.insert(std::make_pair("Num3", sf::Keyboard::Num3));
	button.insert(std::make_pair("Num4", sf::Keyboard::Num4));
	button.insert(std::make_pair("Num5", sf::Keyboard::Num5));
	button.insert(std::make_pair("Num6", sf::Keyboard::Num6));
	button.insert(std::make_pair("Num7", sf::Keyboard::Num7));
	button.insert(std::make_pair("Num8", sf::Keyboard::Num8));
	button.insert(std::make_pair("Num9", sf::Keyboard::Num9));
	button.insert(std::make_pair("Escape", sf::Keyboard::Escape));
	button.insert(std::make_pair("LControl", sf::Keyboard::LControl));
	button.insert(std::make_pair("LShift", sf::Keyboard::LShift));
	button.insert(std::make_pair("LAlt", sf::Keyboard::LAlt));
	button.insert(std::make_pair("LSystem", sf::Keyboard::LSystem));
	button.insert(std::make_pair("RControl", sf::Keyboard::RControl));
	button.insert(std::make_pair("RShift", sf::Keyboard::RShift));
	button.insert(std::make_pair("RAlt", sf::Keyboard::RAlt));
	button.insert(std::make_pair("RSystem", sf::Keyboard::RSystem));
	button.insert(std::make_pair("Menu", sf::Keyboard::Menu));
	button.insert(std::make_pair("LBracket", sf::Keyboard::LBracket));
	button.insert(std::make_pair("RBracket", sf::Keyboard::RBracket));
	button.insert(std::make_pair("SemiColon", sf::Keyboard::SemiColon));
	button.insert(std::make_pair("Comma", sf::Keyboard::Comma));
	button.insert(std::make_pair("Period", sf::Keyboard::Period));
	button.insert(std::make_pair("Quote", sf::Keyboard::Quote));
	button.insert(std::make_pair("Slash", sf::Keyboard::Slash));
	button.insert(std::make_pair("BackSlash", sf::Keyboard::BackSlash));
	button.insert(std::make_pair("Tilde", sf::Keyboard::Tilde));
	button.insert(std::make_pair("Equal", sf::Keyboard::Equal));
	button.insert(std::make_pair("Dash", sf::Keyboard::Dash));
	button.insert(std::make_pair("Space", sf::Keyboard::Space));
	button.insert(std::make_pair("BackSpace", sf::Keyboard::BackSpace));
	button.insert(std::make_pair("Tab", sf::Keyboard::Tab));
	button.insert(std::make_pair("PageDown", sf::Keyboard::PageUp));
	button.insert(std::make_pair("End", sf::Keyboard::End));
	button.insert(std::make_pair("Home", sf::Keyboard::Home));
	button.insert(std::make_pair("Insert", sf::Keyboard::Insert));
	button.insert(std::make_pair("Delete", sf::Keyboard::Delete));
	button.insert(std::make_pair("Add", sf::Keyboard::Add));
	button.insert(std::make_pair("Subtract", sf::Keyboard::Subtract));
	button.insert(std::make_pair("Multiply", sf::Keyboard::Multiply));
	button.insert(std::make_pair("Divide", sf::Keyboard::Divide));
	button.insert(std::make_pair("Left", sf::Keyboard::Left));
	button.insert(std::make_pair("Right", sf::Keyboard::Right));
	button.insert(std::make_pair("Up", sf::Keyboard::Up));
	button.insert(std::make_pair("Down", sf::Keyboard::Down));
	button.insert(std::make_pair("Numpad0", sf::Keyboard::Numpad0));
	button.insert(std::make_pair("Numpad1", sf::Keyboard::Numpad1));
	button.insert(std::make_pair("Numpad2", sf::Keyboard::Numpad2));
	button.insert(std::make_pair("Numpad3", sf::Keyboard::Numpad3));
	button.insert(std::make_pair("Numpad4", sf::Keyboard::Numpad4));
	button.insert(std::make_pair("Numpad5", sf::Keyboard::Numpad5));
	button.insert(std::make_pair("Numpad6", sf::Keyboard::Numpad6));
	button.insert(std::make_pair("Numpad7", sf::Keyboard::Numpad7));
	button.insert(std::make_pair("Numpad8", sf::Keyboard::Numpad8));
	button.insert(std::make_pair("Numpad9", sf::Keyboard::Numpad9));
	button.insert(std::make_pair("F1", sf::Keyboard::F1));
	button.insert(std::make_pair("F2", sf::Keyboard::F2));
	button.insert(std::make_pair("F3", sf::Keyboard::F3));
	button.insert(std::make_pair("F4", sf::Keyboard::F4));
	button.insert(std::make_pair("F5", sf::Keyboard::F5));
	button.insert(std::make_pair("F6", sf::Keyboard::F6));
	button.insert(std::make_pair("F7", sf::Keyboard::F7));
	button.insert(std::make_pair("F8", sf::Keyboard::F8));
	button.insert(std::make_pair("F9", sf::Keyboard::F9));
	button.insert(std::make_pair("F10", sf::Keyboard::F10));
	button.insert(std::make_pair("F11", sf::Keyboard::F11));
	button.insert(std::make_pair("F12", sf::Keyboard::F12));
	button.insert(std::make_pair("F13", sf::Keyboard::F13));
	button.insert(std::make_pair("F14", sf::Keyboard::F14));
	button.insert(std::make_pair("F15", sf::Keyboard::F15));
	button.insert(std::make_pair("Pause", sf::Keyboard::Pause));

	/*button.insert(std::make_pair(sf::Keyboard::A, "A"));
	button.insert(std::make_pair(sf::Keyboard::B, "B"));
	button.insert(std::make_pair(sf::Keyboard::C, "C"));
	button.insert(std::make_pair(sf::Keyboard::D, "D"));
	button.insert(std::make_pair(sf::Keyboard::E, "E"));
	button.insert(std::make_pair(sf::Keyboard::F, "F"));
	button.insert(std::make_pair(sf::Keyboard::G, "G"));
	button.insert(std::make_pair(sf::Keyboard::H, "H"));
	button.insert(std::make_pair(sf::Keyboard::I, "I"));
	button.insert(std::make_pair(sf::Keyboard::J, "J"));
	button.insert(std::make_pair(sf::Keyboard::K, "K"));
	button.insert(std::make_pair(sf::Keyboard::L, "L"));
	button.insert(std::make_pair(sf::Keyboard::M, "M"));
	button.insert(std::make_pair(sf::Keyboard::N, "N"));
	button.insert(std::make_pair(sf::Keyboard::O, "O"));
	button.insert(std::make_pair(sf::Keyboard::Q, "Q"));
	button.insert(std::make_pair(sf::Keyboard::R, "R"));
	button.insert(std::make_pair(sf::Keyboard::S, "S"));
	button.insert(std::make_pair(sf::Keyboard::T, "T"));
	button.insert(std::make_pair(sf::Keyboard::U, "U"));
	button.insert(std::make_pair(sf::Keyboard::W, "W"));
	button.insert(std::make_pair(sf::Keyboard::X, "X"));
	button.insert(std::make_pair(sf::Keyboard::Y, "Y"));
	button.insert(std::make_pair(sf::Keyboard::Z, "Z"));
	button.insert(std::make_pair(sf::Keyboard::Num0, "Num0"));
	button.insert(std::make_pair(sf::Keyboard::Num1, "Num1"));
	button.insert(std::make_pair(sf::Keyboard::Num2, "Num2"));
	button.insert(std::make_pair(sf::Keyboard::Num3, "Num3"));
	button.insert(std::make_pair(sf::Keyboard::Num4, "Num4"));
	button.insert(std::make_pair(sf::Keyboard::Num5, "Num5"));
	button.insert(std::make_pair(sf::Keyboard::Num6, "Num6"));
	button.insert(std::make_pair(sf::Keyboard::Num7, "Num7"));
	button.insert(std::make_pair(sf::Keyboard::Num8, "Num8"));
	button.insert(std::make_pair(sf::Keyboard::Num9, "Num9"));
	button.insert(std::make_pair(sf::Keyboard::Escape, "Escape"));
	button.insert(std::make_pair(sf::Keyboard::LControl, "LControl"));
	button.insert(std::make_pair(sf::Keyboard::LShift, "LShift"));
	button.insert(std::make_pair(sf::Keyboard::LAlt, "LAlt"));
	button.insert(std::make_pair(sf::Keyboard::LSystem, "LSystem"));
	button.insert(std::make_pair(sf::Keyboard::RControl, "RControl"));
	button.insert(std::make_pair(sf::Keyboard::RShift, "RShift"));
	button.insert(std::make_pair(sf::Keyboard::RAlt, "RAlt"));
	button.insert(std::make_pair(sf::Keyboard::RSystem, "RSystem"));
	button.insert(std::make_pair(sf::Keyboard::Menu, "Menu"));
	button.insert(std::make_pair(sf::Keyboard::LBracket, "LBracket"));
	button.insert(std::make_pair(sf::Keyboard::RBracket, "RBracket"));
	button.insert(std::make_pair(sf::Keyboard::SemiColon, "SemiColon"));
	button.insert(std::make_pair(sf::Keyboard::Comma, "Comma"));
	button.insert(std::make_pair(sf::Keyboard::Period, "Period"));
	button.insert(std::make_pair(sf::Keyboard::Quote, "Quote"));
	button.insert(std::make_pair(sf::Keyboard::Slash, "Slash"));
	button.insert(std::make_pair(sf::Keyboard::BackSlash, "BackSlash"));
	button.insert(std::make_pair(sf::Keyboard::Tilde, "Tilde"));
	button.insert(std::make_pair(sf::Keyboard::Equal, "Equal"));
	button.insert(std::make_pair(sf::Keyboard::Dash, "Dash"));
	button.insert(std::make_pair(sf::Keyboard::Space, "Space"));
	button.insert(std::make_pair(sf::Keyboard::BackSpace, "BackSpace"));
	button.insert(std::make_pair(sf::Keyboard::Tab, "Tab"));
	button.insert(std::make_pair(sf::Keyboard::PageUp, "PageDown"));
	button.insert(std::make_pair(sf::Keyboard::End, "End"));
	button.insert(std::make_pair(sf::Keyboard::Home, "Home"));
	button.insert(std::make_pair(sf::Keyboard::Insert, "Insert"));
	button.insert(std::make_pair(sf::Keyboard::Delete, "Delete"));
	button.insert(std::make_pair(sf::Keyboard::Add, "Add"));
	button.insert(std::make_pair(sf::Keyboard::Subtract, "Subtract"));
	button.insert(std::make_pair(sf::Keyboard::Multiply, "Multiply"));
	button.insert(std::make_pair(sf::Keyboard::Divide, "Divide"));
	button.insert(std::make_pair(sf::Keyboard::Left, "Left"));
	button.insert(std::make_pair(sf::Keyboard::Right, "Right"));
	button.insert(std::make_pair(sf::Keyboard::Up, "Up"));
	button.insert(std::make_pair(sf::Keyboard::Down, "Down"));
	button.insert(std::make_pair(sf::Keyboard::Numpad0, "Numpad0"));
	button.insert(std::make_pair(sf::Keyboard::Numpad1, "Numpad1"));
	button.insert(std::make_pair(sf::Keyboard::Numpad2, "Numpad2"));
	button.insert(std::make_pair(sf::Keyboard::Numpad3, "Numpad3"));
	button.insert(std::make_pair(sf::Keyboard::Numpad4, "Numpad4"));
	button.insert(std::make_pair(sf::Keyboard::Numpad5, "Numpad5"));
	button.insert(std::make_pair(sf::Keyboard::Numpad6, "Numpad6"));
	button.insert(std::make_pair(sf::Keyboard::Numpad7, "Numpad7"));
	button.insert(std::make_pair(sf::Keyboard::Numpad8, "Numpad8"));
	button.insert(std::make_pair(sf::Keyboard::Numpad9, "Numpad9"));
	button.insert(std::make_pair(sf::Keyboard::F1, "F1"));
	button.insert(std::make_pair(sf::Keyboard::F2, "F2"));
	button.insert(std::make_pair(sf::Keyboard::F3, "F3"));
	button.insert(std::make_pair(sf::Keyboard::F4, "F4"));
	button.insert(std::make_pair(sf::Keyboard::F5, "F5"));
	button.insert(std::make_pair(sf::Keyboard::F6, "F6"));
	button.insert(std::make_pair(sf::Keyboard::F7, "F7"));
	button.insert(std::make_pair(sf::Keyboard::F8, "F8"));
	button.insert(std::make_pair(sf::Keyboard::F9, "F9"));
	button.insert(std::make_pair(sf::Keyboard::F10, "F10"));
	button.insert(std::make_pair(sf::Keyboard::F11, "F11"));
	button.insert(std::make_pair(sf::Keyboard::F12, "F12"));
	button.insert(std::make_pair(sf::Keyboard::F13, "F13"));
	button.insert(std::make_pair(sf::Keyboard::F14, "F14"));
	button.insert(std::make_pair(sf::Keyboard::F15, "F15"));
	button.insert(std::make_pair(sf::Keyboard::Pause, "Pause"));*/
}

void Player::respawn(){

	//setting the body fixture to collide with nothing.
	b2Fixture* f = mBody->GetFixtureList()->GetNext();
	b2Filter filter;
	filter.maskBits = ~PLAYER; 
	filter.categoryBits = PLAYER; //I am a PLAYER
	f->SetFilterData(filter);

	//respawn the player to a random position.
	mBody->SetTransform(b2Vec2((rand()/(int)PIXELS_PER_METER)%(GAMEFIELD_WIDTH/(int)PIXELS_PER_METER),
		(rand()/(int)PIXELS_PER_METER)%(GAMEFIELD_HEIGHT/(int)PIXELS_PER_METER)), 0);
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
  		
  		//setting the body fixture to collide with nothing.
		b2Fixture* f = s->getBody()->GetFixtureList();
		b2Filter filter;
		filter.maskBits = ~BOUNDARY; //I collide with boundary only 
		f->SetFilterData(filter);
		
		mGame->getSceneNode()->attachChild(std::dynamic_pointer_cast<SceneNode>(s));

		//Make it fly
		s->setAlive(true);

		float a = (rand()%100)/100.0f;

		float angle = a*i; //The direction is random enough as i is radians.

		b2Body* body = s->getBody();
		float x = mBody->GetPosition().x+direction*sin(angle+a)*0.5f*a*GUN_BARREL_LENGTH; 
		float y = mBody->GetPosition().y+cos(angle+a)*0.5f*a*GUN_BARREL_LENGTH;
		body->SetTransform(b2Vec2(x,y), 0);

		float velX = sin(angle)*8.0f*a;
		float velY = cos(angle)*8.0f*a;

		body->SetLinearVelocity(b2Vec2(velX, velY));
	}
	bloodToSpill = 0;
}

bool Player::isWaitingForRespawn() const{
	return waitingForRespawn;
}