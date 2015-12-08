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

	std::shared_ptr<Powerup> powerup = std::make_shared<Powerup>(this,"texture/ak47.png");
	sceneNode->attachChild(std::static_pointer_cast<SceneNode>(powerup));


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
			options->draw(rWindow, player1->keyNames, player2->keyNames);
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
	//sceneNode->removeAll();
	//delete mGameWorld;
	
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
    	else {
	    	switch (event.type) {
		        // "close requested" event: we close the window
		        case sf::Event::Closed:
		            running = false;
		            break;
	            case sf::Event::KeyPressed:
	            	//Keyboard inputs with delay between presses



	            	//tässä voisi olla tyyliin player1->getKey("scrollweapon")
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
	}

    if(!menu->showScreen) {

    	player1->handleUserInput();
    	player2->handleUserInput();

		sceneNode->updateAll(deltaTime);

 	   mGameWorld->Step(deltaTime.asSeconds(), velocityIterations, positionIterations);
 	}
}

void Game::render() {
	//foreach entity call render
	gamefield->draw(rWindow);
	sceneNode->drawAll(rWindow);
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
							whichKeyPressed(player1->keys[0], player1->keyNames[0]);
							break;
						case 2:
							whichKeyPressed(player1->keys[1], player1->keyNames[1]);
							break;
						case 3:
							whichKeyPressed(player1->keys[2], player1->keyNames[2]);
							break;
						case 4:
							whichKeyPressed(player1->keys[3], player1->keyNames[3]);
							break;
						case 5:
							whichKeyPressed(player1->keys[4], player1->keyNames[4]);
							break;
						case 6:
							whichKeyPressed(player1->keys[5], player1->keyNames[5]);
							break;
						case 7:
							whichKeyPressed(player1->keys[6], player1->keyNames[6]);
							break;
						case 9:
							whichKeyPressed(player2->keys[0], player2->keyNames[0]);
							break;
						case 10:
							whichKeyPressed(player2->keys[1], player2->keyNames[1]);
							break;
						case 11:
							whichKeyPressed(player2->keys[2], player2->keyNames[2]);
							break;
						case 12:
							whichKeyPressed(player2->keys[3], player2->keyNames[3]);
							break;
						case 13:
							whichKeyPressed(player2->keys[4], player2->keyNames[4]);
							break;
						case 14:
							whichKeyPressed(player2->keys[5], player2->keyNames[5]);
							break;
						case 15:
							whichKeyPressed(player2->keys[6], player2->keyNames[6]);
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

void Game::whichKeyPressed(sf::Keyboard::Key &key, std::string &s) {
	bool selected = 1;
	while (selected) {
		for (auto it = player1->button.begin(); it != player1->button.end(); it++) {
			if (sf::Keyboard::isKeyPressed(it->second)) {
				std::ifstream buttons;
				buttons.open("buttons.txt");
				std::ofstream newButtons;
				newButtons.open("newButtons.txt");
				std::string line;
				for (unsigned int i = 0; i < (PLAYER_KEYS * 2); i++) {
					std::getline(buttons, line);
					if (line == s) {
						newButtons << it->first << std::endl;
					}
					else if (i < (PLAYER_KEYS * 2 - 1)) {
						newButtons << line << std::endl;
					}
					else {
						newButtons << line;
					}
				}
				buttons.close();
				newButtons.close();
				if (remove("buttons.txt")) {
					std::cout << "Error removing file." << std::endl;
				}
				if (rename("newButtons.txt", "buttons.txt")) {
					std::cout << "Error renaming file." << std::endl;
				}
				key = it->second;
				s = it->first;
				selected = 0;
			}
		}
	}
}

void Game::gameOver(int winner){
	std::cout << "Player"<<winner<<" Wins!"<< std::endl;
}

Game::~Game(){
	sceneNode->removeAll();
	delete mGameWorld;
}