#ifndef OPTIONS_HH
#define OPTIONS_HH

#include "SFML/Graphics.hpp"

#define MAX_NUMBER_OF_ITEMS 3

class Options {
public:
	Options(int width, int height);
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