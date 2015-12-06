#include "MyContactListener.hpp"
#include "Player.hpp"


void MyContactListener::BeginContact(b2Contact* contact) {

	//This code block is used for the collision detection. Ie. if a banana and a player collide,
	//both entities' startContact() -methods are called with the other entity as an argument.
	//It's okay to call both, as only the projectiles reduce the player's health and for example player's startContact
	//does nothing. 
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();//User datassa on entity eli esim. Banana tai Player..
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	static_cast<Entity*>(bodyUserDataA)->startContact(static_cast<Entity*>(bodyUserDataB));
	static_cast<Entity*>(bodyUserDataB)->startContact(static_cast<Entity*>(bodyUserDataA));


	//This is used for sensing the player's ability to jump.
	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

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

	(void) bodyUserDataA;
	(void) bodyUserDataB;

	//This is used for sensing the player's ability to jump.
	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

	if((long)fixtureUserDataA == PLAYER_FOOT_SENSOR_FIXTURE && (long)fixtureUserDataB == GROUND_FIXTURE) {
		static_cast<Player*>(bodyUserDataA)->updateGroundContacts(-1);
	}
	if((long)fixtureUserDataB == PLAYER_FOOT_SENSOR_FIXTURE && (long)fixtureUserDataA == GROUND_FIXTURE) {
		static_cast<Player*>(bodyUserDataB)->updateGroundContacts(-1);
	}
	
}
