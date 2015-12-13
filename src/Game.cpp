#include "Game.hpp"

namespace Textures {
	enum ID {
		LandScape,
		Player,
		Bullet
	};
}

Game::Game() {
	mGame = this;
	menu = std::make_shared<Menu>(this);
	options = std::make_shared<Options>(this);
	rWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BITS_PER_PIXEL), "Lieburo");

	while(playing){

		//create the Box2D world
		b2Vec2 gravity(0.0f, 15.0f);
		mGameWorld = new b2World(gravity, true);
		mGameWorld->SetContactListener(&myContactListenerInstance);

		//instantiate the main window

		running = true;

		sceneNode = std::make_shared<SceneNode>();
		player1 = std::make_shared<Player>(this, 2);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(player1));
		player2 = std::make_shared<Player>(this, 1);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(player2));

		std::shared_ptr<Powerup> gravityPowerup = std::make_shared<GravityPU>(this,"texture/weight.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(gravityPowerup));
		std::shared_ptr<Powerup> gravityPowerup2 = std::make_shared<GravityPU>(this,"texture/weight.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(gravityPowerup2));

		std::shared_ptr<Powerup> gravityInverter = std::make_shared<GravityInverter>(this,"texture/arrow.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(gravityInverter));
		
		std::shared_ptr<Powerup> healthRecovery = std::make_shared<HealthRecovery>(this,"texture/heart.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(healthRecovery));
		std::shared_ptr<Powerup> healthRecovery2 = std::make_shared<HealthRecovery>(this,"texture/heart.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(healthRecovery2));
		std::shared_ptr<Powerup> healthRecovery3 = std::make_shared<HealthRecovery>(this,"texture/heart.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(healthRecovery3));

		std::shared_ptr<Powerup> bomb = std::make_shared<Bomb>(this,"texture/heart.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(bomb));


		gamefield = std::make_shared<Gamefield>(mGameWorld);

		//construct gui
		gui = std::make_shared<GUI>(mGame);
		//construct menu and options screens

		run();
		
	}

	rWindow.close();
}
void Game::createRandomPowerup(){
	int randomNumber = rand() % 4;

	if(randomNumber == 0){
		std::shared_ptr<Powerup> gravityPowerupR = std::make_shared<GravityPU>(mGame,"texture/weight.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(gravityPowerupR));
	}
	if(randomNumber == 1){
		std::shared_ptr<Powerup> gravityInverterR = std::make_shared<GravityInverter>(mGame,"texture/arrow.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(gravityInverterR));
	}
	if(randomNumber == 2){
		std::shared_ptr<Powerup> healthRecoveryR = std::make_shared<HealthRecovery>(mGame,"texture/heart.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(healthRecoveryR));
	}
	if(randomNumber == 3){
		std::shared_ptr<Powerup> bombR = std::make_shared<Bomb>(mGame,"texture/heart.png",true);
		sceneNode->attachChild(std::static_pointer_cast<SceneNode>(bombR));
	}
}

//main game loop
void Game::run() {
	sf::Clock clock;

	//double currentTime = clock.getElapsedTime().asSeconds();
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	//fixed fps game loop, http://gafferongames.com/game-physics/fix-your-timestep/

	//create views for players
	viewMenu.reset(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	view1.setViewport(sf::FloatRect(0, 0, 0.495f, 1.0f-STATUS_PROCENT));
	view1.setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT*(1.0f-STATUS_PROCENT));
	view2.setViewport(sf::FloatRect(0.505f, 0, 0.495f, 1.0f-STATUS_PROCENT));
	view2.setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT*(1.0f-STATUS_PROCENT));

	//third smaller viewport for displaying healthbar and other vital info about the game
	//all the statusbars are located far away from playing field
	statusView.setViewport(sf::FloatRect(0, 1.0f - STATUS_PROCENT, 1.0f, STATUS_PROCENT));
	statusView.setSize(SCREEN_WIDTH, SCREEN_HEIGHT*STATUS_PROCENT);
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
		else{
			view1.setCenter(limitPlayerCamera(player1, view1));
			rWindow.setView(view1);
			render();
			rWindow.setView(view2);
			view2.setCenter(limitPlayerCamera(player2, view2));
			render();
			rWindow.setView(statusView);
			gui->update();
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
		            exit();
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
	if(!menu->isScreenShown() && !options->isScreenShown()) {
    	player1->handleUserInput();
    	player2->handleUserInput();

	 	powerupTimer++;
	 	if(powerupTimer == 3600){
	 		createRandomPowerup();
	 		powerupTimer=0;
	 	}

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

	float h = player->getAnimatedSprite().getLocalBounds().height/4;
	if(pos.x<viewSize.x/2.f){
		pos.x = viewSize.x/2.f;
	}else if(pos.x>GAMEFIELD_WIDTH-viewSize.x/2.f)
		pos.x = GAMEFIELD_WIDTH-viewSize.x/2.f;

	if(pos.y<viewSize.y/2.f+h){
		pos.y = viewSize.y/2.f+h;
	}else if(pos.y>GAMEFIELD_HEIGHT-viewSize.y/2.f+h)
		pos.y = GAMEFIELD_HEIGHT-viewSize.y/2.f+h;

	return pos;
}

void Game::newGame(){
	running = false;
	rWindow.clear();
}

void Game::exit(){
	playing = false;
	running = false;
}