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


	//check the type of fixture A 
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();//User datassa on entity eli esim. Banana tai Player..
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	int idA = 0, idB = 0;

	if(bodyUserDataA) { //the playground has the bodyUserData of 0.
		idA = static_cast<Entity*>(bodyUserDataA)->getType();
	}
	if(bodyUserDataB) {
		idB = static_cast<Entity*>(bodyUserDataB)->getType();
	}

	//std::cout<<"idA:"<<idA<<"idB:"<<idB<<std::endl;

	if(idA == PLAYER || idA == BANANA) { 
		static_cast<Entity*>(bodyUserDataA)->startContact(idB);
	}

	if(idB == PLAYER || idB == BANANA) {
		static_cast<Entity*>(bodyUserDataB)->startContact(idA);
	}

}

void MyContactListener::EndContact(b2Contact* contact) {
	
	(void) contact;
}
