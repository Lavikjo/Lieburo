#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Game.hpp"
#include <sstream>
#include <cassert>

class GUI {
public:
	GUI();
	~GUI();
	void update(Game* game);
	void draw(sf::RenderTarget &target);

private:

	void createBar(sf::RectangleShape &bar, sf::Vector2f pos, sf::Vector2f size, size_t outlineThickness);
	void createText(sf::Text &text, sf::Vector2f pos, size_t outlineThickness);
	int clamp(int n, int lower, int upper);
	sf::Font statusFont;
	sf::Text hpText1, ammoText1, jetText1, hpText2, ammoText2, jetText2;
	sf::RectangleShape healthBar1, healthBar2;

	static const int HP1_TEXT_OFFSET = 99640;
	static const int AMMO1_TEXT_OFFSET = 99750;
	static const int JET1_TEXT_OFFSET = 99900;
	static const int HP1_BAR_OFFSET = 99500;

	static const int HP2_TEXT_OFFSET = 100320;
	static const int AMMO2_TEXT_OFFSET = 100150;
	static const int JET2_TEXT_OFFSET = 100010;
	static const int HP2_BAR_OFFSET = 100400;

};