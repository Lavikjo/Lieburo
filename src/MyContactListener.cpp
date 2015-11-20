#include "MyContactListener.hpp"

void MyContactListener::BeginContact(b2Contact* contact) {
  
	//HUOMHOUM!!! miten static cast toimii?
	/*
		//Katso jos on Ball (iforce2D:n tutoriaaleissa käytetään palloa)
		if ( bodyUserData )
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		static_cast<Ball*>( bodyUserData )->startContact();
	*/

		/*
		COLLISION MASKIT PITÄÄ LAITTAA OK!
		*/

	//check the type of fixture A 
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();//User datassa on entity eli esim. Banana tai Player..
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

	(void) bodyUserDataA;
	(void) bodyUserDataB;

}

void MyContactListener::EndContact(b2Contact* contact) {
	(void) contact;
}
