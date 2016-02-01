// Object.h
// Authors:		Andrew VanKooten
// Description:	An abstract class used to represent objects in a store.
// Assumptions:	None.
#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

class Object
{
	// operator<<
	// Description:	Calls the virtual display method
	// Preconditions:	obj is initialized
	// Postconditions:	The Object is displayed.
	friend ostream& operator<<(ostream& OutStream, const Object& obj);

protected:
	char type;

public:
	//Default Constructor
	Object();
	//Default Destructor
	virtual ~Object();
	
	// Getter
	// Returns a char representing the object type
	char getType();

	// create()
	// Description:	Pure virtual function. This function returns a common pointer type which points to an
	//	object of a derived class.
	// Preconditions:	The descriptions needs to be formatted correctly, determined
	//	by the class that is implementing it.
	// Postconditions:	An Object* is returned, pointing to an object whose class inherits
	//	from the Object class.
	virtual Object* create(string& description) = 0;

	// display()
	// Description: Pure virtual function. This function calls the derived classes display function
	// Preconditions: None
	// Postconditions: The derived objects data is displayed
	virtual void display() const = 0;

	// Comparison Operators
	// Description:	These operators are pure virtual functions that call the derived classes 
	//	comparison operators
	// Preconditions:	The comparable data members of both the calling object and the
	//	 parameter object should be initialized.
	// Postconditions:	A boolean is returned.
	virtual bool operator<(const Object& obj) const = 0;
	virtual bool operator>(const Object& obj) const = 0; 
	virtual bool operator==(const Object& obj) const = 0; 
	virtual bool operator!=(const Object& obj) const = 0;
	virtual bool operator<=(const Object& obj) const = 0;
	virtual bool operator>=(const Object& obj) const = 0;


};

