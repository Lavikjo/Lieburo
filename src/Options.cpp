#include "Options.hpp"

Options::Options(int width, int height) {
	if (!font.loadFromFile("orbitron-black.ttf")) {
		//TODO handle error
	}

	options[0].setFont(font);
	options[0].setColor(sf::Color::Red);
	options[0].setString("Player 1 options");
	options[0].setPosition(sf::Vector2f(width / 4, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	options[1].setFont(font);
	options[1].setColor(sf::Color::White);
	options[1].setString("Move up");
	options[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	options[2].setFont(font);
	options[2].setColor(sf::Color::White);
	options[2].setString("Move down");
	options[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

	selectedItemIndex = 0;
	showScreen = true;
}

Options::~Options() {

}

void Options::draw(sf::RenderWindow &window) {
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) {
		window.draw(options[i]);
	}
}

void Options::moveUp() {
	if (selectedItemIndex - 1 >= 0) {
		options[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex--;
		options[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void Options::moveDown() {
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS) {
		options[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex++;
		options[selectedItemIndex].setColor(sf::Color::Red);
	}
}

int Options::getPressedItem() {
	return selectedItemIndex;
}