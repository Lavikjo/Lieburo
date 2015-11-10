#include "Player.hpp"



 Player::Player(b2World* mWorld){
	
	/*
	^^ Onko palautusarvona &, * vai joku muu
	Huomhuom, nyt body ja fixture alustetaan minne sattuu. 
	*/

	//Create the dynamic body
	body_def.type = b2_dynamicBody;
	body_def.position.Set(30, 1);
	body_def.angle = 0;
	body = mWorld->CreateBody(&body_def);

	//Add a fixture to the body
	b2PolygonShape boxShape;
	boxShape.SetAsBox(1,1);
	fixture_def.shape = &boxShape;
	fixture_def.density = 1;
	body->CreateFixture(&fixture_def);

	// Declare and load a texture
	texture.loadFromFile("Astronaut-1.png");
	
	// Create a sprite
	sprite.setTexture(texture);
	sprite.setPosition(100, 25);
}

void Player::update(){

}
