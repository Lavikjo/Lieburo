#include "Options.hpp"

Options::Options() {
	if (!font.loadFromFile("orbitron-black.ttf")) {
		//TODO handle error
	}


	options[0].setString("Player 1 buttons");
	options[1].setString("Jetpack");
	options[2].setString("Move left");
	options[3].setString("Move right");
	options[4].setString("Aim up");
	options[5].setString("Aim down");
	options[6].setString("Shoot");
	options[7].setString("Change weapon");

	options[8].setString("Player 2 buttons");
	options[9].setString("Jetpack");
	options[10].setString("Move left");
	options[11].setString("Move right");
	options[12].setString("Aim up");
	options[13].setString("Aim down");
	options[14].setString("Shoot");
	options[15].setString("Change weapon");	
	options[16].setString("Return to Main Menu");

	setText();

	selectedItemIndex = 16;
	showScreen = false;
}

Options::~Options() {

}

void Options::draw(sf::RenderWindow &window, std::vector<std::string> keyNames1, std::vector<std::string> keyNames2) {
	for (int i = 0; i < MAX_NUMBER_OF_OPTIONS_ITEMS; i++) {
		window.draw(options[i]);
	}
	for (unsigned int j = 0; j < 7; j++) {
		keys[j].setString(keyNames1[j]);
		keys2[j].setString(keyNames2[j]);

		window.draw(keys[j]);
		window.draw(keys2[j]);
	}
}

void Options::moveUp() {
	if (selectedItemIndex > 1 && selectedItemIndex != 9) {
		options[selectedItemIndex].setColor(sf::Color::White);	
		selectedItemIndex--;
		options[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void Options::moveDown() {
	if (selectedItemIndex == 7) {
		options[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex = 16;
		options[selectedItemIndex].setColor(sf::Color::Red);
	}
	else if (selectedItemIndex + 1 < MAX_NUMBER_OF_OPTIONS_ITEMS) {
		options[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex++;
		options[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void Options::moveLeft() {
	if (selectedItemIndex > (MAX_NUMBER_OF_OPTIONS_ITEMS - 9) && selectedItemIndex != (MAX_NUMBER_OF_OPTIONS_ITEMS - 1)) {
		options[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex += -8;
		options[selectedItemIndex].setColor(sf::Color::Red);
	}
}

void Options::moveRight() {
	if (selectedItemIndex < (MAX_NUMBER_OF_OPTIONS_ITEMS - 9) && selectedItemIndex != 0) {
		options[selectedItemIndex].setColor(sf::Color::White);
		selectedItemIndex += 8;
		options[selectedItemIndex].setColor(sf::Color::Red);
	}
}

int Options::getPressedItem() {
	return selectedItemIndex;
}

void Options::setText() {
	for (unsigned int i = 0; i < MAX_NUMBER_OF_OPTIONS_ITEMS; i++) {
		options[i].setFont(font);
		if (i < 8) {
			if (i == 0) {
				options[i].setCharacterSize(37);
			}
			else {
				options[i].setCharacterSize(28);
			}
			options[i].setColor(sf::Color::White);
			options[i].setPosition(sf::Vector2f(SCREEN_WIDTH / 20.0f, (SCREEN_HEIGHT / (MAX_NUMBER_OF_OPTIONS_ITEMS - 7) * (i + 1))));		
		}
		else if (8 <= i && i < 16) {
			if (i == 8) {
				options[i].setCharacterSize(37);
			}
			else {
				options[i].setCharacterSize(28);
			}
			options[i].setColor(sf::Color::White);
			options[i].setPosition(sf::Vector2f(SCREEN_WIDTH / 20.0f * 11.0f, (SCREEN_HEIGHT / (MAX_NUMBER_OF_OPTIONS_ITEMS - 7) * (i - 7))));					
		}
		else {
			options[i].setColor(sf::Color::Red);
			options[i].setCharacterSize(40);
			sf::FloatRect textRect = options[i].getLocalBounds();
			options[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			options[i].setPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, (SCREEN_HEIGHT / (MAX_NUMBER_OF_OPTIONS_ITEMS - 7) * (i - 7))));		
		}
	}
	for (unsigned int j = 0; j < 7; j++) {
		keys[j].setFont(font);
		keys[j].setColor(sf::Color::White);
		keys[j].setPosition(sf::Vector2f(SCREEN_WIDTH / 20.0f * 8.0f, (SCREEN_HEIGHT / 10 * (j + 2))));
		keys[j].setCharacterSize(28);

		keys2[j].setFont(font);
		keys2[j].setColor(sf::Color::White);
		keys2[j].setPosition(sf::Vector2f(SCREEN_WIDTH / 20.0f * 16.0f, (SCREEN_HEIGHT / 10 * (j + 2))));
		keys2[j].setCharacterSize(28);
	}
}