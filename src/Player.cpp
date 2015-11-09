#include "Player.hpp"

Player::Player Player::initializePlayer(b2World mWorld){
	// Declare and load a texture
	texture.loadFromFile("Astronaut-1.png");
	// Create a sprite
	sprite.setTexture(texture);
	sprite.setPosition(100, 25);


}

Player::update(){

}

Player::synchronize(){
	
}
