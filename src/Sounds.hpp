#ifndef SOUNDS_HH
#define SOUNDS_HH

#include <SFML/Audio.hpp>

#include "Game.hpp"

class Sounds {
public:
	Sounds(Game*);
	~Sounds() {}
	void play();
	void pause();
	void stop();
	bool isPlaying();

private:
	Game* mGame;
	sf::Music music;
	bool playing;
};

#endif