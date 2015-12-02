#pragma once

#include "Game.hpp"

class MyQueryCallback : public b2QueryCallback {
public: 
	std::vector<b2Body*> foundBodies;

	bool ReportFixture(b2Fixture* fixture){
		foundBodies.push_back(fixture->GetBody());
		return true;
	}
};	