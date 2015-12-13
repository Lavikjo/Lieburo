#ifndef SOUNDS_HH
#define SOUNDS_HH

#include <SFML/Audio.hpp>

#include "Game.hpp"

class Sounds {
public:
	Sounds(Game*);
	~Sounds() {}
	void play(sf::Music &music);
	void pause(sf::Music &music);
	void stop(sf::Music &music);
	sf::Music& getMenuMusic();
	sf::Music& getGameMusic();

private:
	Game* mGame;
	sf::Music menuBackground;
	sf::Music gameBackground;
};

#endif