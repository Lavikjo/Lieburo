#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Game.hpp"
#include "Player.hpp"
#include <sstream>
#include <cassert>
class Player;

class GUI {
public:
	GUI(Game* game);
	~GUI();
	void update();
	void draw(sf::RenderTarget &target);

private:

	std::shared_ptr<Player> player1;
	std::shared_ptr<Player> player2;
	Game* mGame;
	void createBar(sf::RectangleShape &bar, sf::Vector2f pos, sf::Vector2f size, size_t outlineThickness, sf::Color color = sf::Color(105,105, 105, 255));
	void createText(sf::Text &text, sf::Vector2f pos, size_t outlineThickness);
	int clamp(int n, int lower, int upper);
	sf::Font statusFont;
	sf::Text livesText1, jetText1, livesText2, jetText2, respawn1, respawn2;
	sf::RectangleShape healthBar1, healthBar2, ammoBar1, ammoBar2;
	sf::Sprite background;
	sf::Texture backgroundTexture;

};