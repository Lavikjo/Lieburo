#ifndef OPTIONS_HH
#define OPTIONS_HH

#include "SFML/Graphics.hpp"
#include "Constants.h"
#include <vector>
#include <string>

class Options {
public:
	Options();
	~Options();

	void draw(sf::RenderWindow &window, std::vector<std::string> keyNames1, std::vector<std::string> keyNames2);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	int getPressedItem();
	bool showScreen;
	void setText();
	sf::Text keys[PLAYER_KEYS];
	sf::Text keys2[PLAYER_KEYS];

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text options[MAX_NUMBER_OF_OPTIONS_ITEMS];
};

#endif