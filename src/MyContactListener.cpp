#include "MyContactListener.hpp"

void MyContactListener::BeginContact(b2Contact* contact) {
	//check the type of fixture A 
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();//User datassa on entity eli esim. Banana tai Player..
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	(void) bodyUserDataA;
	(void) bodyUserDataB;

}

void MyContactListener::EndContact(b2Contact* contact) {
	
	(void) contact;
}
