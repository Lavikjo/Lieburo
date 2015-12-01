#include "Game.hpp"
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
	b2Vec2 gravity(0.0f, 9.8f);
	mGameWorld = new b2World(gravity, true);
	mGameWorld->SetContactListener(&myContactListenerInstance);

	//instantiate the main window
	rWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BITS_PER_PIXEL), "Lieburo");


	running = true;

	sceneNode = std::make_shared<SceneNode>();
	player1 = std::make_shared<Player>(this, 2);
	sceneNode->attachChild(std::static_pointer_cast<SceneNode>(player1));
	player2 = std::make_shared<Player>(this, 1);
	sceneNode->attachChild(std::static_pointer_cast<SceneNode>(player2));
/*
	healthBar1.setOutlineColor(sf::Color::Green);
	healthBar1.setFillColor(sf::Color(0,220, 0, 255));
	healthBar1.setOutlineThickness(15);
	healthBar1.setSize(sf::Vector2f(100,15));
	healthBar1.setPosition(99600,100000);

	healthBar2.setOutlineColor(sf::Color::Green);
	healthBar2.setFillColor(sf::Color(0,220, 0, 255));
	healthBar2.setOutlineThickness(15);
	healthBar2.setSize(sf::Vector2f(100,15));
	healthBar2.setPosition(100300,100000);

*/

	gamefield = std::make_shared<Gamefield>(mGameWorld);

	//construct gui
	gui = std::make_shared<GUI>();

	//construct menu and options screens
	menu = std::make_shared<Menu>();
	options = std::make_shared<Options>();

	run();

	rWindow.close();
}

//main game loop
void Game::run() {
	sf::Clock clock;

	//double currentTime = clock.getElapsedTime().asSeconds();
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	//fixed fps game loop, http://gafferongames.com/game-physics/fix-your-timestep/

	//create views for players
	viewMenu.reset(sf::FloatRect(0, 0, 1024, 768));
	view1.setViewport(sf::FloatRect(0, 0, 0.5f, 0.925f));
	view1.setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT*0.925f);
	view2.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 0.925f));
	view2.setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT*0.925f);

	//third smaller viewport for displaying healthbar and other vital info about the game
	//all the statusbars are located far away from playing field
	statusView.setViewport(sf::FloatRect(0, 0.925f, 1.0f, 0.075f));
	statusView.setSize(SCREEN_WIDTH, SCREEN_HEIGHT*0.075f);
	statusView.setCenter(100000, 100000);


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
		while(clock.getElapsedTime() < TIMESTEP) {
			for(int i = 0; i<10000; i++);
			//Process system events
			//processEvents();

			//update game entities
		}
		update(clock.getElapsedTime());
		// render entities
		rWindow.clear();
		if (menu->showScreen) {
			rWindow.setView(viewMenu);
			menu->draw(rWindow);
		}
		else if (options->showScreen) {
			options->draw(rWindow);
		}
		else {
			view1.setCenter(player1->returnPosition());
			rWindow.setView(view1);
			render();
			rWindow.setView(view2);
			view2.setCenter(player2->returnPosition());
			render();
			rWindow.setView(statusView);
			gui->update(this);
			render();
		}
		rWindow.display();

		// check wheter user wants to enter menu screen (using button P)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
	    	menu->showScreen = true;
		}
	}
}

void Game::update(sf::Time deltaTime) {
	//foreach entity call update

	sf::Event event;

    while (rWindow.pollEvent(event)) {

    	//navigate in menu screen
	  	if (menu->showScreen) {
	  		navigateMenu(event);
    	}
    	else if (options->showScreen) {
    		navigateOptions(event);
    	}

    	switch (event.type) {
	        // "close requested" event: we close the window
	        case sf::Event::Closed:
	            running = false;
	            break;
            case sf::Event::KeyPressed:
            	//Keyboard inputs with delay between presses
			    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
			    	//std::cout << "jump1" <<std::endl;
				    player2->scrollWeapons();
				}
			    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
				    player1->scrollWeapons();
				}
            	break;
	        default:
	        	break;
	    }

	}

    if(!menu->showScreen) {
    	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
	    	//std::cout << "jump1" <<std::endl;
		    player1->jump();
		}
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		    player1->movePlayerX(-0.1f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		    player1->movePlayerX(0.1f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		    player1->fire();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		    player1->aim(3);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		    player1->aim(-3);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		    player2->jump();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
		    player2->movePlayerX(-0.1f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		    player2->movePlayerX(0.1f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
		    player2->fire();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
		    player2->aim(3);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
		    player2->aim(-3);
		}

		sceneNode->updateAll(deltaTime);

 	   mGameWorld->Step(deltaTime.asSeconds(), velocityIterations, positionIterations);
 	}
}

void Game::render() {
	//foreach entity call render
	sceneNode->drawAll(rWindow);
	gamefield->draw(rWindow);
	gui->draw(rWindow);
/*	
	//TODO: Move this to better place
	
	
	rWindow.draw(healthBar1);

	
	
	rWindow.draw(healthBar2);
*/
}

b2World* Game::getWorld(){
	return mGameWorld;
}

std::shared_ptr<SceneNode> Game::getSceneNode(){
	return sceneNode;
}

void Game::navigateMenu(sf::Event &event) {
	switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
				case sf::Keyboard::Up:
					menu->moveUp();
					break;
				case sf::Keyboard::Down:
					menu->moveDown();
					break;
				case sf::Keyboard::Return:
					switch (menu->getPressedItem()) {
						case 0:
							std::cout << "User pressed Play button." << std::endl;
							menu->showScreen = false;
							break;
						case 1:
							std::cout << "User pressed Options button." << std::endl;
							menu->showScreen = false;
							options->showScreen = true;
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

void Game::navigateOptions(sf::Event &event) {
	switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
				case sf::Keyboard::Up:
					options->moveUp();
					break;
				case sf::Keyboard::Down:
					options->moveDown();
					break;
				case sf::Keyboard::Return:
					switch (options->getPressedItem()) {
						case 0:
							std::cout << "User pressed first button." << std::endl;
							options->showScreen = false;
							menu->showScreen = true;
							break;
						case 1:
							std::cout << "User pressed second button." << std::endl;
							break;;
						case 2:
							std::cout << "User pressed second button." << std::endl;
							break;
					}
				default:
					break;
			}
			default:
				break;
	}
}
/*
sf::RenderWindow Game::getRenderWindow() {
	return rWindow;
}
*/

std::shared_ptr<Player> Game::getPlayer(int id){
	if(id == 1){
		return player1;
	}
	else if(id == 2) {
		return player2;
	}

	return nullptr;
}
