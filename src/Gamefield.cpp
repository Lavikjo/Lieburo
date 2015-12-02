#include "Gamefield.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Gamefield::Gamefield(b2World* world) {
	/*
	1. Create the static body

	2. Load the gamefield data from a file by going it through line by line
		File data structure: (location unit is pixels)
		Texture1name,x1,y1\n
		Texture2name,x2,y2\n

		ATM the only choices are a box of 60*30 pixels. Maybe in the future there will be other kinds of shapes (= non-box fixtures)
		as well as other textures, too.

	3. Per one line: Create a texture and create a sprite from it. Push the texture and sprite to a vector.

	4. Create a fixture based on the data (= pos + upcoming dimensios/shape information) in the file.

	5. Attach the fixture to the body.

	6. Repeat steps 3-5.  
	

	*/

	//Step 1
	mBodyDef.type = b2_staticBody;
	mBodyDef.position.Set(0, 0);//up-left corner
	mBodyDef.fixedRotation = true; // prevent rotation
	mBody = world->CreateBody(&mBodyDef);
	mBody->SetUserData(this); //emun would be better

	
	std::cout<<"Initializing gamefield" << std::endl;

	//Step 2
	std::string line;
	std::ifstream gameFieldFile ("gamefield1");

	if (gameFieldFile.is_open()) {
		while (std::getline(gameFieldFile,line)) {
			//Steps 3-5

			//splitting the line at the "," 

			std::stringstream ss(line);
			std::vector<std::string>fields;
			std::string field = "";
			while (std::getline(ss, field, ',')) {
				fields.push_back(field);
			}

		    std::string filename = fields[0];
		    int x = std::stoi(fields[1], nullptr);
		    int y = std::stoi(fields[2], nullptr);

		    // Texture and sprite
		    loadTexture(filename);
			
		    sf::Sprite mSprite;
		    mSprite.setTexture(mGroundTextures[filename]);
	    	
			//setting the origin to be in the center as it is with the Box2d bodies and fixtures.
			sf::FloatRect bounds = mSprite.getLocalBounds();
			mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
			mSprite.setPosition(x, y);
			mGroundSprites.push_back(mSprite);
		

		    //creating the fixture
		    b2PolygonShape polygonShape;
		    b2FixtureDef mFixtureDef;
			polygonShape.SetAsBox(0.5f*bounds.width / PIXELS_PER_METER, 0.5f*bounds.height/PIXELS_PER_METER, b2Vec2(x/PIXELS_PER_METER,y/PIXELS_PER_METER), 0);
			mFixtureDef.shape = &polygonShape;
			mFixtureDef.density = 1;
			mFixtureDef.friction = 1.0f;
			mFixtureDef.filter.categoryBits = BOUNDARY; //I am a BOUNDARY, I collide with everything.
			mBody->CreateFixture(&mFixtureDef);
		}
	gameFieldFile.close();
	}
}

Gamefield::~Gamefield() {

}

void Gamefield::draw(sf::RenderTarget& target) {
	for(auto& groundSprite:mGroundSprites) {
		target.draw(groundSprite);
	}
}

void Gamefield::loadTexture(std::string filename){
	sf::Texture t;
	t.loadFromFile(filename);
	mGroundTextures[filename] = t; 
}

int Gamefield::getType() {
	return BOUNDARY;
}