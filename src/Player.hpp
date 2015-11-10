#include "Entity.hpp"

class Player : public Entity {
public:
	Player();
	~Player();

	Player initializePlayer(b2World* mWorld);
	void update();
	void render();
};