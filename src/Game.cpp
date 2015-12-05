#include "Game.hpp"

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

	//set data for keyboard input
	setButtons();

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
			    if (sf::Keyboard::isKeyPressed(player1->keys[6])) {
				    	player1->scrollWeapons();
				}
			    if (sf::Keyboard::isKeyPressed(player2->keys[6])) {
			   		player2->scrollWeapons();
				}
            	break;
	        default:
	        	break;
	    }

	}

    if(!menu->showScreen) {
    	if (sf::Keyboard::isKeyPressed(player1->keys[0])) {
		    player1->jump();
		}
	    if (sf::Keyboard::isKeyPressed(player1->keys[1])) {
		    player1->movePlayerX(-0.1f);
		}
		if (sf::Keyboard::isKeyPressed(player1->keys[2])) {
		    player1->movePlayerX(0.1f);
		}
		if (sf::Keyboard::isKeyPressed(player1->keys[3])) {
		    player1->aim(3);
		}
		if (sf::Keyboard::isKeyPressed(player1->keys[4])) {
		    player1->aim(-3);
		}
		if (sf::Keyboard::isKeyPressed(player1->keys[5])) {
		    player1->fire();
		}
		if (sf::Keyboard::isKeyPressed(player2->keys[0])) {
		    player2->jump();
		}
		if (sf::Keyboard::isKeyPressed(player2->keys[1])) {
		    player2->movePlayerX(-0.1f);
		}
		if (sf::Keyboard::isKeyPressed(player2->keys[2])) {
		    player2->movePlayerX(0.1f);
		}
		if (sf::Keyboard::isKeyPressed(player2->keys[3])) {
		    player2->aim(3);
		}
		if (sf::Keyboard::isKeyPressed(player2->keys[4])) {
		    player2->aim(-3);
		}
		if (sf::Keyboard::isKeyPressed(player2->keys[5])) {
		    player2->fire();
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
				case sf::Keyboard::Left:
					options->moveLeft();
					break;
				case sf::Keyboard::Right:
					options->moveRight();
					break;

				case sf::Keyboard::Return:
					switch (options->getPressedItem()) {
						case 1:
							whichKeyPressed(player1->keys[0]);
							break;
						case 2:
							whichKeyPressed(player1->keys[1]);
							break;
						case 3:
							whichKeyPressed(player1->keys[2]);
							break;
						case 4:
							whichKeyPressed(player1->keys[3]);
							break;
						case 5:
							whichKeyPressed(player1->keys[4]);
							break;
						case 6:
							whichKeyPressed(player1->keys[5]);
							break;
						case 7:
							whichKeyPressed(player1->keys[6]);
							break;
						case 9:
							whichKeyPressed(player2->keys[0]);
							break;
						case 10:
							whichKeyPressed(player2->keys[1]);
							break;
						case 11:
							whichKeyPressed(player2->keys[2]);
							break;
						case 12:
							whichKeyPressed(player2->keys[3]);
							break;
						case 13:
							whichKeyPressed(player2->keys[4]);
							break;
						case 14:
							whichKeyPressed(player2->keys[5]);
							break;
						case 15:
							whichKeyPressed(player2->keys[6]);
							break;
						case 16:
							std::cout << "User pressed Return to main menu." << std::endl;
							options->showScreen = false;
							menu->showScreen = true;
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

void Game::whichKeyPressed(sf::Keyboard::Key &key) {

	for (auto it = button.begin(); it != button.end(); it++) {
		if (sf::Keyboard::isKeyPressed(it->first)) {
			std::cout << "testi" << std::endl;
			key = it->first;
		}
	}
}

void Game::setButtons() {
	button.insert(std::make_pair(sf::Keyboard::A, "A"));
	button.insert(std::make_pair(sf::Keyboard::B, "B"));
	button.insert(std::make_pair(sf::Keyboard::C, "C"));
	button.insert(std::make_pair(sf::Keyboard::D, "D"));
	button.insert(std::make_pair(sf::Keyboard::E, "E"));
	button.insert(std::make_pair(sf::Keyboard::F, "F"));
	button.insert(std::make_pair(sf::Keyboard::G, "G"));
	button.insert(std::make_pair(sf::Keyboard::H, "H"));
	button.insert(std::make_pair(sf::Keyboard::I, "J"));
	button.insert(std::make_pair(sf::Keyboard::K, "K"));
	button.insert(std::make_pair(sf::Keyboard::L, "L"));
	button.insert(std::make_pair(sf::Keyboard::M, "M"));
	button.insert(std::make_pair(sf::Keyboard::N, "N"));
	button.insert(std::make_pair(sf::Keyboard::O, "O"));
	button.insert(std::make_pair(sf::Keyboard::P, "P"));
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
	button.insert(std::make_pair(sf::Keyboard::Pause, "Pause"));
}