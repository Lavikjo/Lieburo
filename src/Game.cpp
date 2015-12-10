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

	menu = std::make_shared<Menu>(this);
	options = std::make_shared<Options>(this);

	sceneNode = std::make_shared<SceneNode>();
	player1 = std::make_shared<Player>(this, 2);
	sceneNode->attachChild(std::static_pointer_cast<SceneNode>(player1));
	player2 = std::make_shared<Player>(this, 1);
	sceneNode->attachChild(std::static_pointer_cast<SceneNode>(player2));

	std::shared_ptr<Powerup> powerup = std::make_shared<Powerup>(this,"texture/ak47.png");
	sceneNode->attachChild(std::static_pointer_cast<SceneNode>(powerup));


	gamefield = std::make_shared<Gamefield>(mGameWorld);

	//construct gui
	gui = std::make_shared<GUI>();
	//construct menu and options screens

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
	viewMenu.reset(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
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
			//Delay for not to check the time every nanosecond or so.
			for(int i = 0; i<10000; i++);
		}
		update(clock.getElapsedTime());
		// render entities
		rWindow.clear();
		if (menu->isScreenShown()) {
			rWindow.setView(viewMenu);
			menu->draw(rWindow);
		}
		else if (options->isScreenShown()) {
			options->draw(rWindow, player1->getKeyNames(), player2->getKeyNames());
		}
		else {			
			sf::Vector2f pos = player1->getSpritePosition();
			sf::Vector2f viewSize = view1.getSize();
			if(pos.x<viewSize.x/2.f){
				pos.x = viewSize.x/2.f;
			}else if(pos.x>GAMEFIELD_WIDTH-viewSize.x/2.f)
				pos.x = GAMEFIELD_WIDTH-viewSize.x/2.f;

			if(pos.y<viewSize.y/2.f){
				pos.y = viewSize.y/2.f;
			}else if(pos.y>GAMEFIELD_HEIGHT-viewSize.y/2.f)
				pos.y = GAMEFIELD_HEIGHT-viewSize.y/2.f;

			view1.setCenter(limitPlayerCamera(player1, view1));
			rWindow.setView(view1);
			render();
			rWindow.setView(view2);
			view2.setCenter(limitPlayerCamera(player2, view2));
			render();
			rWindow.setView(statusView);
			gui->update(this);
			render();
		}
		rWindow.display();

		// check wheter user wants to enter menu screen (using button P)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
	    	menu->setScreenShown(true);
		}
	}	
}

void Game::update(sf::Time deltaTime) {
	sf::Event event;

    while (rWindow.pollEvent(event)) {

    	//navigate in menu screen
	  	if (menu->isScreenShown()) {
	  		menu->navigateMenu(event);
    	}
    	else if (options->isScreenShown()) {
    		options->navigateOptions(event);
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
				    if (sf::Keyboard::isKeyPressed(player1->getKeys()[6])) { 
					    player1->scrollWeapons();
					}
				    if (sf::Keyboard::isKeyPressed(player2->getKeys()[6])) {
				   		player2->scrollWeapons();
					}
	            	break;
		        default:
		        	break;
		    }
		}

	}
	if(!menu->isScreenShown()) {

    	player1->handleUserInput();
    	player2->handleUserInput();

		sceneNode->updateAll(deltaTime);

 	   mGameWorld->Step(deltaTime.asSeconds(), velocityIterations, positionIterations);
 	}
}

void Game::render() {
	gamefield->draw(rWindow);
	sceneNode->drawAll(rWindow);
	gui->draw(rWindow);
}

b2World* Game::getWorld(){
	return mGameWorld;
}

std::shared_ptr<SceneNode> Game::getSceneNode(){
	return sceneNode;
}


sf::RenderWindow& Game::getRenderWindow() {
	return rWindow;
}


std::shared_ptr<Player> Game::getPlayer(int id){
	if(id == 1){
		return player1;
	}
	else if(id == 2) {
		return player2;
	}

	return nullptr;
}

void Game::gameOver(int winner){
	std::cout << "Player"<<winner<<" Wins!"<< std::endl;
}

Game::~Game(){
	sceneNode->removeAll();
	delete mGameWorld;
}

std::shared_ptr<Menu> Game::getMenu() {
	return menu;
}

std::shared_ptr<Options> Game::getOptions() {
	return options;
}

void Game::setRunning(bool run){
	running = run;
}

sf::Vector2f Game::limitPlayerCamera(std::shared_ptr<Player> player, sf::View view){
	sf::Vector2f pos = player->getSpritePosition();
	sf::Vector2f viewSize = view.getSize();
	if(pos.x<viewSize.x/2.f){
		pos.x = viewSize.x/2.f;
	}else if(pos.x>GAMEFIELD_WIDTH-viewSize.x/2.f)
		pos.x = GAMEFIELD_WIDTH-viewSize.x/2.f;

	if(pos.y<viewSize.y/2.f){
		pos.y = viewSize.y/2.f+20;
	}else if(pos.y>GAMEFIELD_HEIGHT-viewSize.y/2.f)
		pos.y = GAMEFIELD_HEIGHT-viewSize.y/2.f+20;

	return pos;
}