#ifndef GAME_HH
#define GAME_HH

//library includes
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


//include required objects
//#include "Player.hpp"

class Game {
public:
	Game(){
		running = true;
	}
	~Game(){}
	void setup();
	void run();
	void update(sf::Time deltaTime);
	void render();
private:
	bool running;
	
};

#endif