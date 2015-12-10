#ifndef OPTIONS_HH
#define OPTIONS_HH

#include "SFML/Graphics.hpp"
#include "Constants.h"
#include "Game.hpp"
#include <vector>
#include <string>

const unsigned int NUMBER_PLAYER_KEYS = 7;
const unsigned int MAX_NUMBER_OF_OPTIONS_ITEMS = 17;

class Options {
public:
	Options(Game*);
	~Options();

	void draw(sf::RenderWindow &window, std::vector<std::string> keyNames1, std::vector<std::string> keyNames2);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	int getPressedItem();
	bool isScreenShown();
	void setScreenShown(bool);
	void setText();
	void navigateOptions(sf::Event &event);
	void setButtons();
	void whichKeyPressed(sf::Keyboard::Key &key, std::string &s, unsigned int a);
	int getNumberPlayerKeys();
	std::map<std::string, sf::Keyboard::Key> getButtons();
	

private:
	sf::Text player1Keys[NUMBER_PLAYER_KEYS];
	sf::Text player2Keys[NUMBER_PLAYER_KEYS];
	bool screenShown;
	std::map<std::string, sf::Keyboard::Key> buttons;
	unsigned int selectedItemIndex;
	sf::Font font;
	sf::Text options[MAX_NUMBER_OF_OPTIONS_ITEMS];
	Game* mGame;
};

#endif