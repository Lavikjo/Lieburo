	#include "Menu.hpp"

Menu::Menu() {
	if (!font.loadFromFile("orbitron-black.ttf")) {
		//TODO handle error
	}

	menu[0].setFont(font);
	menu[0].setColor(sf::Color::Red);
	menu[0].setString("Play");

	menu[1].setFont(font);
	menu[1].setColor(sf::Color::White);
	menu[1].setString("Options");

	menu[2].setFont(font);
	menu[2].setColor(sf::Color::White);
	menu[2].setString("Exit");

	setPositions();

	selectedItemIndex = 0;
	showScreen = true;
}

Menu::~Menu() {


}

void Menu::draw(sf::RenderWindow &window) {
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) {
		window.draw(menu[i]);
	}
}

void Menu::moveUp() {
	if (selectedItemIndex - 1 >= 0) {
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void Menu::moveDown() {
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS) {
		menu[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setColor(sf::Color::Red);
	}
}

int Menu::getPressedItem() {
	return selectedItemIndex;
}

void Menu::setPositions() {
	for (unsigned int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) {
		sf::FloatRect textRect = menu[i].getLocalBounds();
		menu[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		menu[i].setPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, (SCREEN_HEIGHT / (MAX_NUMBER_OF_ITEMS + 1) * (i + 1))));
	}
}