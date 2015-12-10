	#include "Menu.hpp"

Menu::Menu(Game* game) {
	mGame = game;
	if (!font.loadFromFile("orbitron-black.ttf")) {
		//TODO handle error
	}

	menu[0].setFont(font);
	menu[0].setColor(sf::Color::Red);
	menu[0].setString("New Game");

	menu[1].setFont(font);
	menu[1].setColor(sf::Color::White);
	menu[1].setString("Resume Game");

	menu[2].setFont(font);
	menu[2].setColor(sf::Color::White);
	menu[2].setString("Options");

	menu[3].setFont(font);
	menu[3].setColor(sf::Color::White);
	menu[3].setString("Exit");

	setPositions();

	selectedItemIndex = 0;
	screenShown = true;
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

void Menu::navigateMenu(sf::Event &event) {
	switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
				case sf::Keyboard::Up:
					moveUp();
					break;
				case sf::Keyboard::Down:
					moveDown();
					break;
				case sf::Keyboard::Return:
					switch (getPressedItem()) {
						case 0:
							std::cout << "User pressed New Game button." << std::endl;
							//TODO: Implement game reset here
							break;
						case 1:
							std::cout << "User pressed Resume Game button." << std::endl;
							screenShown = false;
							break;
						case 2:
							std::cout << "User pressed Options button." << std::endl;
							screenShown = false;
							mGame->getOptions()->setScreenShown(true);
							break;
						case 3:
							mGame->setRunning(false);
							break;
					}
				default:
					break;
			}
			default:
				break;
	}
}

bool Menu::isScreenShown(){
	return screenShown;
}

void Menu::setScreenShown(bool is){
	screenShown = is;
}