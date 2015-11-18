#ifndef GAME_HH
#define GAME_HH

//library includes
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


//include required objects
//#include "Player.hpp"
#include "Menu.hpp"

class Game {
public:
	Game();
	~Game(){}
	void run();
	void update(sf::Time deltaTime, Menu &menu);
	void render();
	void synchronize();
private:
	bool running;
	bool menu_screen;
	sf::RenderWindow rWindow;
	
};

#endif