#include "GUI.hpp"


GUI::GUI() {

	bool success = statusFont.loadFromFile("orbitron-black.ttf");

	//if the font cannot be loaded, program fails instantly
	assert(success);

	//GUI for player1
	createBar(healthBar1, sf::Vector2f(HP1_BAR_OFFSET, 100000), sf::Vector2f(100, 15), 15);
	createText(hpText1, sf::Vector2f(HP1_TEXT_OFFSET, 100000), 15);
	createText(ammoText1, sf::Vector2f(AMMO1_TEXT_OFFSET, 100000), 15);
	createText(jetText1, sf::Vector2f(JET1_TEXT_OFFSET, 100000), 15);


	//GUI for player2
	createBar(healthBar2, sf::Vector2f(HP2_BAR_OFFSET, 100000), sf::Vector2f(100, 15), 15);
	createText(hpText2, sf::Vector2f(HP2_TEXT_OFFSET, 100000), 15);
	createText(ammoText2, sf::Vector2f(AMMO2_TEXT_OFFSET, 100000), 15);
	createText(jetText2, sf::Vector2f(JET2_TEXT_OFFSET, 100000), 15);
	
	
}

GUI::~GUI() {

}

void GUI::draw(sf::RenderTarget &target){

	target.draw(healthBar1);
	target.draw(healthBar2);
	target.draw(jetText1);
	target.draw(jetText2);
	target.draw(ammoText1);
	target.draw(ammoText2);
	target.draw(hpText1);
	target.draw(hpText2);
}

void GUI::update(Game* game) {
	std::stringstream amount;

	std::shared_ptr<Player> player1 = game->getPlayer(1);
	std::shared_ptr<Player> player2 = game->getPlayer(2);
	float currentHp1 = static_cast<float>(player1->getHp());
	float currentHp2 = static_cast<float>(player2->getHp());

	//sanity checks, so that negative values are not drawn
	if(currentHp1 >= 0) {

		amount << currentHp1;
		hpText1.setString(amount.str());
		amount.str("");
		amount.clear();

		healthBar1.setScale(currentHp1/100, 1);
	}
	if(currentHp2 >= 0) {
		amount << currentHp2;
		hpText2.setString(amount.str());
		amount.str("");
		amount.clear();

		healthBar2.setScale(currentHp2/100, 1);
	}

	

	amount << player1->getCurrentAmmo() << "/" << player1->getCurrentClipSize();
	ammoText1.setString(amount.str());
	
	// clear stream in between
	amount.str("");
	amount.clear();

	//populate stream again with player2 data
	amount << player2->getCurrentAmmo() << "/" << player2->getCurrentClipSize();
	ammoText2.setString(amount.str());

	amount.str("");
	amount.clear();

	amount << player1->getFuel();
	jetText1.setString(amount.str());

	amount.str("");
	amount.clear();

	amount << player2->getFuel();
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

}


void GUI::createBar(sf::RectangleShape &bar, sf::Vector2f pos, sf::Vector2f size, size_t outlineThickness) {
	bar.setOutlineColor(sf::Color::Green);
	bar.setFillColor(sf::Color(0,220, 0, 255));
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