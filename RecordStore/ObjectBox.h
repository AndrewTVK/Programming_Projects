// ObjectBox.h
//	Authors:		Andrew VanKooten
//	Description:	Is derived from a Box class and holds a pointer to an inventory object 
//	Assumptions:	There will be a Object.h and box.h file
#pragma once
#include "Box.h"
#include "Object.h"
class ObjectBox : public Box
{
public:
	//Default Constructor
	ObjectBox();

	// Overloaded Constructor
	// Description:			Stores the Object pointer from the parameter in data
	// Preconditions:		The accepted Object must point to data on the heap
	// Postconditions:		object will be equal to the parameter
	ObjectBox(Object*);

	//Default Constructor
	virtual ~ObjectBox();

	// Comparison Operators
	// Description: These operators compares box and *this and 
	//	returns a bool based on if that operation is true or not
	// Preconditons: box must point to data on the heap
	// Postconditions: A bool is returned if the operation is true or not
	virtual bool operator<(const Box& box) const;
	virtual bool operator==(const Box& box) const;
	virtual bool operator!=(const Box& box) const;

	// display()
	// displays the object
	// Preconditons: none
	// Postconditions: object is displayed on the console
	virtual void display() const;

protected:
	Object *object = NULL;
};

