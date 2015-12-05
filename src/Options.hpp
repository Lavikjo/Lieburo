#ifndef OPTIONS_HH
#define OPTIONS_HH

#include "SFML/Graphics.hpp"
#include "Constants.h"

#define MAX_NUMBER_OF_OPTIONS_ITEMS 17

class Options {
public:
	Options();
	~Options();

	void draw(sf::RenderWindow &window);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	int getPressedItem();
	bool showScreen;
	void setInformation();

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text options[MAX_NUMBER_OF_OPTIONS_ITEMS];
};

#endif