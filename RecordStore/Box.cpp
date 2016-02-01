// Box.cpp
// Author: Andrew VanKooten
// Description: Abstract class that makes it so inventory objects and customer objects can be stored in BSTree
// Assumptions: None
#include "Box.h"


Box::Box()
{

}

Box::~Box()
{
}

// getHashableValue()
// returns an int holding hashablevalue
int Box::getHashableValue() const { return hashableValue; }

// operator<<
// Description: Displays the contents in box
// Preconditions: box has data initialized
// Postconditions: virtual display method is called
ostream& operator<<(ostream& OutStream, const Box& box)
{
	box.display();
	return OutStream;
}

// getType()
// returns a char representing the type:
char Box::getType() const
{
	return type;
}