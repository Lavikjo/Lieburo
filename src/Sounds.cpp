#include "Sounds.hpp"

Sounds::Sounds(Game* game) {
	mGame = game;

	if (!menuBackground.openFromFile("menu_background.wav")) {
		std::cout << "Couldn't load background music for menu." << std::endl;
	}
	if (!gameBackground.openFromFile("game_background.wav")) {
		std::cout << "Couldn't load background music for game." << std::endl;
	}
	
}

void Sounds::play(sf::Music &music) {
	if (music.getStatus() != sf::SoundSource::Playing) {
		music.play();
	}
}

void Sounds::pause(sf::Music &music) {
	if (music.getStatus() != sf::SoundSource::Paused) {
		music.pause();
	}
}

void Sounds::stop(sf::Music &music) {
	if (music.getStatus() != sf::SoundSource::Stopped) {
		music.stop();
	}
}

sf::Music& Sounds::getMenuMusic() {
	return menuBackground;
}

sf::Music& Sounds::getGameMusic() {
	return gameBackground;
}