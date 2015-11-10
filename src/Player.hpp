#include "Entity.hpp"

class Player : public Entity {
public:
	Player();
	~Player();

	Player initializePlayer(b2World* mWorld);
	void update();
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::Sprite mSprite;

};