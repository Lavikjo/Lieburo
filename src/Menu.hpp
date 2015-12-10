#ifndef MENU_HH
#define MENU_HH

#include "SFML/Graphics.hpp"
#include "Constants.h"

#include "Game.hpp"

#define MAX_NUMBER_OF_ITEMS 3

class Menu {
public:
	Menu(Game*);
	~Menu(){}

	void draw(sf::RenderWindow &window);
	void moveUp();
	void moveDown();
	int getPressedItem();
	void setScreenShown(bool);
	bool isScreenShown();
	void setPositions();
	void navigateMenu(sf::Event &event);

private:
	Game* mGame;
	bool screenShown;
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];
};

#endif