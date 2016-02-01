// ObjectBox.cpp
//	Authors:		Andrew VanKooten
//	Description:	Is derived from a Box class and holds a pointer to an inventory object 
//	Assumptions:	There will be a Object.h and box.h file
#include "ObjectBox.h"

ObjectBox::ObjectBox()
{
}

ObjectBox::~ObjectBox()
{
	delete object;
}

// Overloaded Constructor
// Description:			Stores the Object pointer from the parameter in data
// Preconditions:		The accepted Object must point to data on the heap
// Postconditions:		object will be equal to the parameter
ObjectBox::ObjectBox(Object* dummy)
{
	object = dummy;
	type = 'O';
}

// display()
// displays the object
// Preconditons: none
// Postconditions: object is displayed on the console
void ObjectBox::display() const
{
	cout << *object;
}

// Comparison Operators
// Description: These operators compares box and *this and 
//	returns a bool based on if that operation is true or not
// Preconditons: box must point to data on the heap
// Postconditions: A bool is returned if the operation is true or not
bool ObjectBox::operator<(const Box& box) const
{
	const ObjectBox& objectBox = static_cast<const ObjectBox&>(box);
	if (*object < *objectBox.object) return true;
	else return false;
}

bool ObjectBox::operator==(const Box& box) const
{
	const ObjectBox& objectBox = static_cast<const ObjectBox&>(box);
	return (*object == *objectBox.object);
}

bool ObjectBox::operator!=(const Box& box) const
{
	return !(*this == box);
}