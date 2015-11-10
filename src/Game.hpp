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
	Game();
	~Game(){}
	void run();
	void update(sf::Time deltaTime);
	void render();
	void synchronize();
private:
	bool running;
	sf::RenderWindow rWindow;
	
};

#endif