#ifndef MENU_HH
#define MENU_HH

#include "SFML/Graphics.hpp"
#include "Constants.h"

#define MAX_NUMBER_OF_ITEMS 3

class Menu {
public:
	Menu();
	~Menu();

	void draw(sf::RenderWindow &window);
	void moveUp();
	void moveDown();
	int getPressedItem();
	bool showScreen;
	void setPositions();

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];
};

#endif