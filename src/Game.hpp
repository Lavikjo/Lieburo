#ifndef GAME_HH
#define GAME_HH

//library includes
#include "SFML/Window.hpp"


//include required objects
#include "Weapon.hpp"
#include "Player.hpp"

class Game {
public:
	void setup();
	void run();
	void update(sf::Time deltaTime);
	void render();
private:
	bool running = true;
	
};

#endif