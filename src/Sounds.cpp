#include "Sounds.hpp"

Sounds::Sounds(Game* game) {
	mGame = game;
	playing = false;

	if (!music.openFromFile("backgroundmusic.wav")) {
		std::cout << "Couldn't load background music." << std::endl;
	}
	
}

void Sounds::play() {
	if (!playing) {
		music.play();
		playing = true;
	}
}

void Sounds::pause() {
	if (playing) {
		music.pause();
		playing = false;
	}
}

void Sounds::stop() {
	if (playing) {
		music.stop();
		playing = false;
	}
}

bool Sounds::isPlaying() {
	return playing;
}