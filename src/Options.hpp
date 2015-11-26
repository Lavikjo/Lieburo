#ifndef OPTIONS_HH
#define OPTIONS_HH

#include "SFML/Graphics.hpp"
#include "Constants.h"

#define MAX_NUMBER_OF_ITEMS 3

class Options {
public:
	Options();
	~Options();

	void draw(sf::RenderWindow &window);
	void moveUp();
	void moveDown();
	int getPressedItem();
	bool showScreen;

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text options[MAX_NUMBER_OF_ITEMS];
};

#endif