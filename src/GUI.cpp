#include "GUI.hpp"


GUI::GUI(Game* game) {
	
	//get game and players here for data acquisition
	mGame = game;
	player1 = mGame->getPlayer(1);
	player2 = mGame->getPlayer(2);

	bool success = statusFont.loadFromFile("orbitron-black.ttf");

	//if the font cannot be loaded, program fails instantly
	assert(success);

	
	success = backgroundTexture.loadFromFile("texture/metalbackground1.png");
	assert(success);
	
	background.setTexture(backgroundTexture);
	background.setPosition(sf::Vector2f(100000 - SCREEN_WIDTH/2, 100000 - SCREEN_HEIGHT*STATUS_PROCENT));
	background.setScale(sf::Vector2f(1.5f, 1.0f));

	//GUI for player1
	createBar(healthBar1, sf::Vector2f(100000 - SCREEN_WIDTH/2, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/2.5f), sf::Vector2f(200, 5), 5, sf::Color(0, 255, 0, 255));
	createBar(ammoBar1, sf::Vector2f(100000 - SCREEN_WIDTH/2, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/4.25f), sf::Vector2f(200, 5), 5);
	createText(livesText1, sf::Vector2f(100000 - SCREEN_WIDTH/2 + 400, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/3.5f), 20);
	createText(jetText1, sf::Vector2f(100000 - SCREEN_WIDTH/2, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/9.0f), 30);
	createText(respawn1, sf::Vector2f(100000 - SCREEN_WIDTH/2 + 400, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/9.0f), 20);
	respawn1.setString("Press fire to respawn!");

	//GUI for player2
	createBar(healthBar2, sf::Vector2f(100000 + SCREEN_WIDTH/2, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/2.5f), sf::Vector2f(200, 5), 5, sf::Color(0, 255, 0, 255));
	createBar(ammoBar2, sf::Vector2f(100000 + SCREEN_WIDTH/2, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/4.25f), sf::Vector2f(200, 5), 5);
	createText(livesText2, sf::Vector2f(100000 + SCREEN_WIDTH/2 - 400, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/3.5f), 20);
	createText(jetText2, sf::Vector2f(100000 + SCREEN_WIDTH/2 - 70, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/9.0f), 30);
	createText(respawn2, sf::Vector2f(100000 + SCREEN_WIDTH/2 - 400, 100000 - SCREEN_HEIGHT*STATUS_PROCENT/9.0f), 20);
	respawn2.setString("Press fire to respawn!");
	
}

GUI::~GUI() {

}

void GUI::draw(sf::RenderTarget &target){

	target.draw(background);
	target.draw(respawn1);
	target.draw(respawn2);
	target.draw(healthBar1);
	target.draw(healthBar2);
	target.draw(ammoBar1);
	target.draw(ammoBar2);
	target.draw(jetText1);
	target.draw(jetText2);
	target.draw(livesText1);
	target.draw(livesText2);
}

void GUI::update() {
	std::stringstream amount;

	

	float currentHp1 = static_cast<float>(player1->getHp());
	currentHp1 = clamp(currentHp1, 0, player1->getMaxHp()); //clamp values to avoid negative values

	healthBar1.setScale(currentHp1/player1->getMaxHp(), 1);

	float currentHp2 = static_cast<float>(player2->getHp());
	currentHp2 = clamp(currentHp2, 0, player2->getMaxHp());
	
	healthBar2.setScale(-currentHp2/player2->getMaxHp(), 1);

	amount << "Lives: " << player1->getMaxLives() + player1->getLives();
	livesText1.setString(amount.str());
	amount.str("");
	amount.clear();

	

	amount << "Lives: " << player2->getMaxLives() + player2->getLives();
	livesText2.setString(amount.str());

	amount.str("");
	amount.clear();

	amount << static_cast<int>(player1->getFuel());
	jetText1.setString(amount.str());

	amount.str("");
	amount.clear();

	amount << static_cast<int>(player2->getFuel());
	jetText2.setString(amount.str());


	//green bar on nearly full hp
	if(currentHp1 > 80) {
		healthBar1.setOutlineColor(sf::Color::Green);
		healthBar1.setFillColor(sf::Color(0, 220, 0, 255));

	//yellow bar on mid hp
	} else if (currentHp1 > 40 and currentHp1 < 80) {
		healthBar1.setOutlineColor(sf::Color::Yellow);
		healthBar1.setFillColor(sf::Color(220, 220, 0, 255));

	} else if (currentHp1 < 40) {
		healthBar1.setOutlineColor(sf::Color::Red);
		healthBar1.setFillColor(sf::Color(220, 0, 0, 255));
		}

	if(currentHp2 > 80) {
		healthBar2.setOutlineColor(sf::Color::Green);
		healthBar2.setFillColor(sf::Color(0, 220, 0, 255));

	} else if (currentHp2 > 40 and currentHp2 < 80) {
		healthBar2.setOutlineColor(sf::Color::Yellow);
		healthBar2.setFillColor(sf::Color(220, 220, 0, 255));

	} else if (currentHp2 < 40) {
		healthBar2.setOutlineColor(sf::Color::Red);
		healthBar2.setFillColor(sf::Color(220, 0, 0, 255));
		}

	//get time statistics for smooth reload/ammo indicator bar
	float elapsed1 = player1->getFireClock().getElapsedTime().asSeconds();
	float reloadtime1 = player1->getReloadTime();

	//if player is reloading scale the bar according the time left, else scale with ammunation left
	if(!player1->getCurrentAmmo() && elapsed1 < reloadtime1) {
		ammoBar1.setScale(elapsed1/reloadtime1, 1);
	} else {
		ammoBar1.setScale(static_cast<float>(player1->getCurrentAmmo())/static_cast<float>(player1->getCurrentClipSize()), 1);
	}

	float elapsed2 = player2->getFireClock().getElapsedTime().asSeconds();
	float reloadtime2 = player2->getReloadTime(); 

	if(!player2->getCurrentAmmo() && elapsed2 < reloadtime2) {
		ammoBar2.setScale(-elapsed2/reloadtime2, 1);
	} else {
		ammoBar2.setScale(-static_cast<float>(player2->getCurrentAmmo())/static_cast<float>(player2->getCurrentClipSize()), 1);
	}

	if(player1->isWaitingForRespawn()) {
		respawn1.setColor(sf::Color(255, 0, 0, 255));
	} else {
		respawn1.setColor(sf::Color(255, 0, 0, 0));
	}

	if(player2->isWaitingForRespawn()) {
		respawn2.setColor(sf::Color(255, 0, 0, 255));
	} else {
		respawn2.setColor(sf::Color(255, 0, 0, 0));
	}
}


void GUI::createBar(sf::RectangleShape &bar, sf::Vector2f pos, sf::Vector2f size, size_t outlineThickness, sf::Color color) {
	bar.setOutlineColor(color);
	bar.setFillColor(color);
	bar.setOutlineThickness(outlineThickness);
	bar.setSize(size);
	bar.setPosition(pos);
}

void GUI::createText(sf::Text &text, sf::Vector2f pos, size_t outlineThickness) {
	text.setCharacterSize(outlineThickness);
	text.setStyle(sf::Text::Bold);
	text.setColor(sf::Color::Red);
	text.setPosition(pos);
	text.setFont(statusFont);
	text.setString("0");
}

//helper function for clamping hp values
int GUI::clamp(int n, int lower, int upper) {
	return std::max(lower, std::min(n, upper));
}