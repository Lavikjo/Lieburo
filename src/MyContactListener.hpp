#ifndef MYCONTACTLISTENER_HH
#define MYCONTACTLISTENER_HH

#include <iostream>
#include <Box2D/Box2D.h>

class MyContactListener : public b2ContactListener {
public:
	MyContactListener() = default;
    ~MyContactListener(){}
 
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
};

#endif