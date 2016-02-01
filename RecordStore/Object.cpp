// Object.cpp
// Authors:		Andrew VanKooten
// Description:	An abstract class used to represent objects in a store.
// Assumptions:	None.
#include "Object.h"


Object::Object()
{
}

Object::~Object()
{
}

// operator<<
// Description:	Calls the virtual display method
// Preconditions:	obj is initialized
// Postconditions:	The Object is displayed.
ostream& operator<<(ostream& OutStream, const Object& obj)
{
	obj.display();
	return OutStream;
}

// Returns a char representing the object type
char Object::getType()
{
	return type;
}