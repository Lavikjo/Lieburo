#include "Options.hpp"
#include <iostream>

Options::Options(Game* game) {
	mGame = game;
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
	screenShown = false;

	setButtons();
}

Options::~Options() {

}

void Options::draw(sf::RenderWindow &window, std::vector<std::string> keyNames1, std::vector<std::string> keyNames2) {
	for (unsigned int i = 0; i < MAX_NUMBER_OF_OPTIONS_ITEMS; i++) {
		window.draw(options[i]);
	}
	for (unsigned int j = 0; j < 7; j++) {
		player1Keys[j].setString(keyNames1[j]);
		player2Keys[j].setString(keyNames2[j]);

		window.draw(player1Keys[j]);
		window.draw(player2Keys[j]);
	}

	window.draw(reset[0]);
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
		player1Keys[j].setFont(font);
		player1Keys[j].setColor(sf::Color::White);
		player1Keys[j].setPosition(sf::Vector2f(SCREEN_WIDTH / 20.0f * 8.0f, (SCREEN_HEIGHT / 10 * (j + 2))));
		player1Keys[j].setCharacterSize(28);

		player2Keys[j].setFont(font);
		player2Keys[j].setColor(sf::Color::White);
		player2Keys[j].setPosition(sf::Vector2f(SCREEN_WIDTH / 20.0f * 16.0f, (SCREEN_HEIGHT / 10 * (j + 2))));
		player2Keys[j].setCharacterSize(28);
	}
	reset[0].setFont(font);
	reset[0].setString("Press F12 to reset\nPress Enter to change key");
	reset[0].setColor(sf::Color::White);
	reset[0].setPosition(sf::Vector2f(SCREEN_WIDTH / 20.0f, (SCREEN_HEIGHT / (MAX_NUMBER_OF_OPTIONS_ITEMS - NUMBER_PLAYER_KEYS) * (MAX_NUMBER_OF_OPTIONS_ITEMS - NUMBER_PLAYER_KEYS - 1))));
	reset[0].setCharacterSize(15);
}

void Options::navigateOptions(sf::Event &event) {
	switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
				case sf::Keyboard::Up:
					moveUp();
					break;
				case sf::Keyboard::Down:
					moveDown();
					break;
				case sf::Keyboard::Left:
					moveLeft();
					break;
				case sf::Keyboard::Right:
					moveRight();
					break;
				case sf::Keyboard::F12:
					mGame->getPlayer(1)->resetCommands();
					mGame->getPlayer(2)->resetCommands();
					draw(mGame->getRenderWindow(), mGame->getPlayer(1)->getKeyNames(), mGame->getPlayer(2)->getKeyNames());
					break;

				case sf::Keyboard::Return:
					switch (getPressedItem()) {
						case 1:
							whichKeyPressed(mGame->getPlayer(1)->getKeys()[0], mGame->getPlayer(1)->getKeyNames()[0], 0);
							break;
						case 2:
							whichKeyPressed(mGame->getPlayer(1)->getKeys()[1], mGame->getPlayer(1)->getKeyNames()[1], 1);
							break;
						case 3:
							whichKeyPressed(mGame->getPlayer(1)->getKeys()[2], mGame->getPlayer(1)->getKeyNames()[2], 2);
							break;
						case 4:
							whichKeyPressed(mGame->getPlayer(1)->getKeys()[3], mGame->getPlayer(1)->getKeyNames()[3], 3);
							break;
						case 5:
							whichKeyPressed(mGame->getPlayer(1)->getKeys()[4], mGame->getPlayer(1)->getKeyNames()[4], 4);
							break;
						case 6:
							whichKeyPressed(mGame->getPlayer(1)->getKeys()[5], mGame->getPlayer(1)->getKeyNames()[5], 5);
							break;
						case 7:
							whichKeyPressed(mGame->getPlayer(1)->getKeys()[6], mGame->getPlayer(1)->getKeyNames()[6], 6);
							break;
						case 9:
							whichKeyPressed(mGame->getPlayer(2)->getKeys()[0], mGame->getPlayer(2)->getKeyNames()[0], 7);
							break;
						case 10:
							whichKeyPressed(mGame->getPlayer(2)->getKeys()[1], mGame->getPlayer(2)->getKeyNames()[1], 8);
							break;
						case 11:
							whichKeyPressed(mGame->getPlayer(2)->getKeys()[2], mGame->getPlayer(2)->getKeyNames()[2], 9);
							break;
						case 12:
							whichKeyPressed(mGame->getPlayer(2)->getKeys()[3], mGame->getPlayer(2)->getKeyNames()[3], 10);
							break;
						case 13:
							whichKeyPressed(mGame->getPlayer(2)->getKeys()[4], mGame->getPlayer(2)->getKeyNames()[4], 11);
							break;
						case 14:
							whichKeyPressed(mGame->getPlayer(2)->getKeys()[5], mGame->getPlayer(2)->getKeyNames()[5], 12);
							break;
						case 15:
							whichKeyPressed(mGame->getPlayer(2)->getKeys()[6], mGame->getPlayer(2)->getKeyNames()[6], 13);
							break;
						case 16:
							std::cout << "User pressed Return to main menu." << std::endl;
							screenShown = false;
							mGame->getMenu()->setScreenShown(true);
					}
				default:
					break;
			}
			default:

				break;
	}
}

void Options::whichKeyPressed(sf::Keyboard::Key &key, std::string &s, unsigned int a) {
	bool selected = 1;

	//draw selected key red
	if (a < NUMBER_PLAYER_KEYS) {
		player1Keys[a].setColor(sf::Color::Red);
		mGame->getRenderWindow().draw(player1Keys[a]);
	}
	else {
		player2Keys[a - NUMBER_PLAYER_KEYS].setColor(sf::Color::Red);
		mGame->getRenderWindow().draw(player2Keys[a - NUMBER_PLAYER_KEYS]);
	}
	mGame->getRenderWindow().display();

	while (selected) {
		for (auto it = buttons.begin(); it != buttons.end(); it++) {
			if (sf::Keyboard::isKeyPressed(it->second)) {
				std::ifstream buttonsFile;
				buttonsFile.open("buttons.txt");
				std::ofstream newButtonsFile;
				newButtonsFile.open("newButtons.txt");
				std::string line;
				for (unsigned int i = 0; i < (NUMBER_PLAYER_KEYS * 2); i++) {
					std::getline(buttonsFile, line);
					if (i == a) {
						newButtonsFile << it->first << std::endl;
					}
					else if (i < (NUMBER_PLAYER_KEYS * 2 - 1)) {
						newButtonsFile << line << std::endl;
					}
					else {
						newButtonsFile << line;
					}
				}
				buttonsFile.close();
				newButtonsFile.close();
				if (remove("buttons.txt")) {
					std::cout << "Error removing file." << std::endl;
				}
				if (rename("newButtons.txt", "buttons.txt")) {
					std::cout << "Error renaming file." << std::endl;
				}
				key = it->second;
				s = it->first;
				selected = 0;
			}
		}
	}
	if (a < NUMBER_PLAYER_KEYS) {
		player1Keys[a].setColor(sf::Color::White);
	}
	else {
		player2Keys[a - NUMBER_PLAYER_KEYS].setColor(sf::Color::White);
	}
}

bool Options::isScreenShown() {
	return screenShown;
}

void Options::setScreenShown(bool is) {
	screenShown = is;
}

int Options::getNumberPlayerKeys() {
	return NUMBER_PLAYER_KEYS;
}

std::map<std::string, sf::Keyboard::Key> Options::getButtons() {
	return buttons;
}

void Options::setButtons() {
	buttons.insert(std::make_pair("A", sf::Keyboard::A));
	buttons.insert(std::make_pair("B", sf::Keyboard::B));
	buttons.insert(std::make_pair("C", sf::Keyboard::C));
	buttons.insert(std::make_pair("D", sf::Keyboard::D));
	buttons.insert(std::make_pair("E", sf::Keyboard::E));
	buttons.insert(std::make_pair("F", sf::Keyboard::F));
	buttons.insert(std::make_pair("G", sf::Keyboard::G));
	buttons.insert(std::make_pair("H", sf::Keyboard::H));
	buttons.insert(std::make_pair("I", sf::Keyboard::I));
	buttons.insert(std::make_pair("J", sf::Keyboard::J));
	buttons.insert(std::make_pair("K", sf::Keyboard::K));
	buttons.insert(std::make_pair("L", sf::Keyboard::L));
	buttons.insert(std::make_pair("M", sf::Keyboard::M));
	buttons.insert(std::make_pair("N", sf::Keyboard::N));
	buttons.insert(std::make_pair("O", sf::Keyboard::O));
	buttons.insert(std::make_pair("Q", sf::Keyboard::Q));
	buttons.insert(std::make_pair("R", sf::Keyboard::R));
	buttons.insert(std::make_pair("S", sf::Keyboard::S));
	buttons.insert(std::make_pair("T", sf::Keyboard::T));
	buttons.insert(std::make_pair("U", sf::Keyboard::U));
	buttons.insert(std::make_pair("W", sf::Keyboard::W));
	buttons.insert(std::make_pair("X", sf::Keyboard::X));
	buttons.insert(std::make_pair("Y", sf::Keyboard::Y));
	buttons.insert(std::make_pair("Z", sf::Keyboard::Z));
	buttons.insert(std::make_pair("Num0", sf::Keyboard::Num0));
	buttons.insert(std::make_pair("Num1", sf::Keyboard::Num1));
	buttons.insert(std::make_pair("Num2", sf::Keyboard::Num2));
	buttons.insert(std::make_pair("Num3", sf::Keyboard::Num3));
	buttons.insert(std::make_pair("Num4", sf::Keyboard::Num4));
	buttons.insert(std::make_pair("Num5", sf::Keyboard::Num5));
	buttons.insert(std::make_pair("Num6", sf::Keyboard::Num6));
	buttons.insert(std::make_pair("Num7", sf::Keyboard::Num7));
	buttons.insert(std::make_pair("Num8", sf::Keyboard::Num8));
	buttons.insert(std::make_pair("Num9", sf::Keyboard::Num9));
	buttons.insert(std::make_pair("Escape", sf::Keyboard::Escape));
	buttons.insert(std::make_pair("LControl", sf::Keyboard::LControl));
	buttons.insert(std::make_pair("LShift", sf::Keyboard::LShift));
	buttons.insert(std::make_pair("LAlt", sf::Keyboard::LAlt));
	buttons.insert(std::make_pair("LSystem", sf::Keyboard::LSystem));
	buttons.insert(std::make_pair("RControl", sf::Keyboard::RControl));
	buttons.insert(std::make_pair("RShift", sf::Keyboard::RShift));
	buttons.insert(std::make_pair("RAlt", sf::Keyboard::RAlt));
	buttons.insert(std::make_pair("RSystem", sf::Keyboard::RSystem));
	buttons.insert(std::make_pair("Menu", sf::Keyboard::Menu));
	buttons.insert(std::make_pair("LBracket", sf::Keyboard::LBracket));
	buttons.insert(std::make_pair("RBracket", sf::Keyboard::RBracket));
	buttons.insert(std::make_pair("SemiColon", sf::Keyboard::SemiColon));
	buttons.insert(std::make_pair("Comma", sf::Keyboard::Comma));
	buttons.insert(std::make_pair("Period", sf::Keyboard::Period));
	buttons.insert(std::make_pair("Quote", sf::Keyboard::Quote));
	buttons.insert(std::make_pair("Slash", sf::Keyboard::Slash));
	buttons.insert(std::make_pair("BackSlash", sf::Keyboard::BackSlash));
	buttons.insert(std::make_pair("Tilde", sf::Keyboard::Tilde));
	buttons.insert(std::make_pair("Equal", sf::Keyboard::Equal));
	buttons.insert(std::make_pair("Dash", sf::Keyboard::Dash));
	buttons.insert(std::make_pair("Space", sf::Keyboard::Space));
	buttons.insert(std::make_pair("BackSpace", sf::Keyboard::BackSpace));
	buttons.insert(std::make_pair("Tab", sf::Keyboard::Tab));
	buttons.insert(std::make_pair("PageDown", sf::Keyboard::PageUp));
	buttons.insert(std::make_pair("End", sf::Keyboard::End));
	buttons.insert(std::make_pair("Home", sf::Keyboard::Home));
	buttons.insert(std::make_pair("Insert", sf::Keyboard::Insert));
	buttons.insert(std::make_pair("Delete", sf::Keyboard::Delete));
	buttons.insert(std::make_pair("Add", sf::Keyboard::Add));
	buttons.insert(std::make_pair("Subtract", sf::Keyboard::Subtract));
	buttons.insert(std::make_pair("Multiply", sf::Keyboard::Multiply));
	buttons.insert(std::make_pair("Divide", sf::Keyboard::Divide));
	buttons.insert(std::make_pair("Left", sf::Keyboard::Left));
	buttons.insert(std::make_pair("Right", sf::Keyboard::Right));
	buttons.insert(std::make_pair("Up", sf::Keyboard::Up));
	buttons.insert(std::make_pair("Down", sf::Keyboard::Down));
	buttons.insert(std::make_pair("Numpad0", sf::Keyboard::Numpad0));
	buttons.insert(std::make_pair("Numpad1", sf::Keyboard::Numpad1));
	buttons.insert(std::make_pair("Numpad2", sf::Keyboard::Numpad2));
	buttons.insert(std::make_pair("Numpad3", sf::Keyboard::Numpad3));
	buttons.insert(std::make_pair("Numpad4", sf::Keyboard::Numpad4));
	buttons.insert(std::make_pair("Numpad5", sf::Keyboard::Numpad5));
	buttons.insert(std::make_pair("Numpad6", sf::Keyboard::Numpad6));
	buttons.insert(std::make_pair("Numpad7", sf::Keyboard::Numpad7));
	buttons.insert(std::make_pair("Numpad8", sf::Keyboard::Numpad8));
	buttons.insert(std::make_pair("Numpad9", sf::Keyboard::Numpad9));
	buttons.insert(std::make_pair("F1", sf::Keyboard::F1));
	buttons.insert(std::make_pair("F2", sf::Keyboard::F2));
	buttons.insert(std::make_pair("F3", sf::Keyboard::F3));
	buttons.insert(std::make_pair("F4", sf::Keyboard::F4));
	buttons.insert(std::make_pair("F5", sf::Keyboard::F5));
	buttons.insert(std::make_pair("F6", sf::Keyboard::F6));
	buttons.insert(std::make_pair("F7", sf::Keyboard::F7));
	buttons.insert(std::make_pair("F8", sf::Keyboard::F8));
	buttons.insert(std::make_pair("F9", sf::Keyboard::F9));
	buttons.insert(std::make_pair("F10", sf::Keyboard::F10));
	buttons.insert(std::make_pair("F11", sf::Keyboard::F11));
	buttons.insert(std::make_pair("F13", sf::Keyboard::F13));
	buttons.insert(std::make_pair("F14", sf::Keyboard::F14));
	buttons.insert(std::make_pair("F15", sf::Keyboard::F15));
	buttons.insert(std::make_pair("Pause", sf::Keyboard::Pause));

}