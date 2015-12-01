#include "MyContactListener.hpp"
#include "Player.hpp"


void MyContactListener::BeginContact(b2Contact* contact) {

	/*
	-Collision filtterit
	-Ammuksen hit() ja pelaajan startContact()
	-Guided missile: katsoo aina missä pelaaja on
	-Räjähdykset
	-Ampumiset realistisiksi
	*/


	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();//User datassa on entity eli esim. Banana tai Player..
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

	int idA = 0, idB = 0;


	if(true) { //the playground has the bodyUserData of 0.
		idA = static_cast<Entity*>(bodyUserDataA)->getType();
	}
	if(true) {
		idB = static_cast<Entity*>(bodyUserDataB)->getType();
	}

	//std::cout<<"idA:"<<idA<<"idB:"<<idB<<std::endl;
	if(idA == PLAYER || idA == BANANA) { 
		static_cast<Entity*>(bodyUserDataA)->startContact(idB, static_cast<Entity*>(bodyUserDataB));
	}

	if(idB == PLAYER || idB == BANANA) {
		static_cast<Entity*>(bodyUserDataB)->startContact(idA, static_cast<Entity*>(bodyUserDataA));
	}
	if(idA == MISSILE) { 
		static_cast<Entity*>(bodyUserDataA)->startContact(idB, static_cast<Entity*>(bodyUserDataB));
	}
	if(idB == MISSILE) {
		static_cast<Entity*>(bodyUserDataB)->startContact(idA, static_cast<Entity*>(bodyUserDataA));
	}

	//std::cout << (long)fixtureUserDataA<<", "<< (long)fixtureUserDataB << std::endl;
	if((long)fixtureUserDataA == PLAYER_FOOT_SENSOR_FIXTURE && (long)fixtureUserDataB == GROUND_FIXTURE) {
		static_cast<Player*>(bodyUserDataA)->updateGroundContacts(1);
	}
	if((long)fixtureUserDataB == PLAYER_FOOT_SENSOR_FIXTURE && (long)fixtureUserDataA == GROUND_FIXTURE) {
		static_cast<Player*>(bodyUserDataB)->updateGroundContacts(1);
	}

}

void MyContactListener::EndContact(b2Contact* contact) {
	
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();//User datassa on entity eli esim. Banana tai Player..
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

	(void) bodyUserDataA;
	(void) bodyUserDataB;


	if((long)fixtureUserDataA == PLAYER_FOOT_SENSOR_FIXTURE && (long)fixtureUserDataB == GROUND_FIXTURE) {
		static_cast<Player*>(bodyUserDataA)->updateGroundContacts(-1);
	}
	if((long)fixtureUserDataB == PLAYER_FOOT_SENSOR_FIXTURE && (long)fixtureUserDataA == GROUND_FIXTURE) {
		static_cast<Player*>(bodyUserDataB)->updateGroundContacts(-1);
	}
	
}
