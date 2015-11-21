#include "Game.hpp"
//#include "Constants.h"
#include <iostream>

namespace Textures {
	enum ID {
		LandScape,
		Player,
		Bullet
	};
}

Game::Game() {

	//create the Box2D world
	b2Vec2 gravity(0.0f, 0.0f);
	mGameWorld = new b2World(gravity, true);
	mGameWorld->SetContactListener(&myContactListenerInstance);
	
	//createTerrain();

	//instantiate the main window
	rWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BITS_PER_PIXEL), "Lieburo");

	running = true;

	sceneNode = std::make_shared<SceneNode>();
	player1 = std::make_shared<Player>(this);
	sceneNode->attachChild(std::static_pointer_cast<SceneNode>(player1));

	run();

	rWindow.close();
}

//main game loop
void Game::run() {
	//initialize menu screen
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
		// render entities
		rWindow.clear();
		if (menu.showScreen) {
			menu.draw(rWindow);
		}
		else {
			render();
		}
		rWindow.display();

		// check wheter user wants to enter menu screen (using button P)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
	    	menu.showScreen = true;
		}
	}
}

void Game::update(sf::Time deltaTime, Menu &menu) {
	//foreach entity call update

	sf::Event event;

    while (rWindow.pollEvent(event)) {
    	//navigate in menu screen
	  	if (menu.showScreen) {
    		switch (event.type) {
	    		case sf::Event::KeyPressed:
	    			switch (event.key.code) {
	    				case sf::Keyboard::Up:
	    					menu.MoveUp();
	    					break;

	    				case sf::Keyboard::Down:
	    					menu.MoveDown();
	    					break;

	    				case sf::Keyboard::Return:
	    					switch (menu.GetPressedItem()) {
	    						case 0:
	    							std::cout << "User pressed Play button." << std::endl;
	    							menu.showScreen = false;
	    							break;

	    						case 1:
	    							std::cout << "User pressed Options button." << std::endl;
	    							break;

	    						case 2:
	    							running = false;
	    							break;
	    					}

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

    if(!menu.showScreen) {
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		    player1->movePlayerX(-5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		    player1->movePlayerX(5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		    player1->movePlayerY(-5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		    player1->movePlayerY(5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		    player1->fire();
		    std::cout << "Player1 fired" << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		    player1->aim(3);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		    player1->aim(-3);
		}

		sceneNode->updateAll(deltaTime);

 	   mGameWorld->Step(deltaTime.asSeconds(), velocityIterations, positionIterations);
 	}
}

void Game::render() {
	//foreach entity call render
	sceneNode->drawAll(rWindow);
}

void Game::createTerrain(){
	//create the ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(10.0f, 10.0f);
	b2Body *groundBody = mGameWorld->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(10.0f, 1.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
}

b2World* Game::getWorld(){
	return mGameWorld;
}

std::shared_ptr<SceneNode> Game::getSceneNode(){
	return sceneNode;
}
