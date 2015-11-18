#include "Game.hpp"
#include <iostream> //included for testing purposes

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int BITS_PER_PIXEL = 32;
const sf::Time TIMESTEP = sf::seconds(1.0f / 60.0f); // timestep, 60 fps
const int velocityIterations = 8;
const int positionIterations = 3;



Game::Game() {
	//create the Box2D world
	b2Vec2 gravity(0.0f, -9.8f);
	b2World* mWorld = new b2World(gravity, true);
	//create the ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body *groundBody = mWorld->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//instantiate the main window
	rWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BITS_PER_PIXEL), "Lieburo");
	
	running = true;

	run();


	rWindow.close();
}

//main game loop
void Game::run() {
	//initialize menu screen
	menu_screen = true;
	Menu menu(SCREEN_WIDTH, SCREEN_HEIGHT);


	sf::Clock clock;

	//double currentTime = clock.getElapsedTime().asSeconds();
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	//fixed fps game loop, http://gafferongames.com/game-physics/fix-your-timestep/
	while(running) {
		/*
		double newTime = clock.getElapsedTime().asSeconds();
		double frameTime = newTime - currentTime;
		*/
		sf::Time dt = clock.restart();


		//For avoiding spiral of death
		if(dt > sf::seconds(0.25f)) {
			dt =  sf::seconds(0.25f);
		}

		//currentTime = newTime;
		timeSinceLastUpdate += dt;

		

		//logic update loop, everything that affects physics need to be here
		while(timeSinceLastUpdate > TIMESTEP) {
			
			timeSinceLastUpdate -= TIMESTEP;
			//Process system events
			//processEvents();

			//update game entities
			update(timeSinceLastUpdate, menu);
		}

		// synchronize();
		// render entities
		render();

		rWindow.clear();
		if (menu_screen) {
			menu.draw(rWindow);
		}

		rWindow.display();

		// check wheter user wants to enter menu screen
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
	    	menu_screen = true;
		}
	}
}

void Game::update(sf::Time deltaTime, Menu &menu) {
	//foreach entity call update

	sf::Event event;

    while (rWindow.pollEvent(event)) {
    	//navigate in menu screen
	  	if (menu_screen) {
    		switch (event.type) {
	    		case sf::Event::KeyPressed:
	    			switch (event.key.code) {
	    				case sf::Keyboard::Up:
	    					menu.MoveUp();
	    					break;

	    				case sf::Keyboard::Down:
	    					menu.MoveDown();
	    					break;

	    				default:
	    					break;
	    			}

	    			default:
	    				break;
    		}
    	}

    	switch (event.type) {
	        // "close requested" event: we close the window
	        case sf::Event::Closed:
	            running = false;
	            break;

	        default:
	        	break;
	    }
    }
}

void Game::render() {
	//foreach entity call render
}

void Game::synchronize() {
	//foreach entity call synchronize
}