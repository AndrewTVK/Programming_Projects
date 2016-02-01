// DVD.h
// Author Andrew VanKooten
// Description : Represents a Dvd that is sold in a store. Derived from an Object class
// Assumptions : There will be a Object.h class 

#pragma once

#include "Object.h"

class DVD : public Object
{
protected:
	string director;
	int issueYear;
	string title;

public:
	//Default Constructor
	DVD();

	//Default Destructor
	virtual ~DVD();

	// Overloaded constructor
	// Accepts a string as input and parses that string and uses that info
	//	to initialize its member functions
	// Preconditions: The string must have the required information in it.
	// Postconditions: *this will be set to the info found in the string
	DVD(string description);

	// display()
	//Virtual display method that displays the director, year, and title
	//Preconditions: None
	//Postconditions: The director, title, and yeat will displayed on the console 
	virtual void display() const;

	// Getters
	string getDirector();
	string getTitle();
	int getYear();

	// create()
	// Description: This function returns an Object pointer type which points to an
	//	object of a derived class.
	// Preconditions: The description needs to be formatted correctly, determined
	//	by the class that is implementing it.
	// Postconditions: An Object* is returned, pointing to an DVD object.
	Object* create(string& description);

	// Comparison Operators
	// Description:	These operators compare Dvds first by title, then by year, and then
	//	 by director
	// Preconditions:	The comparable data members of both the calling object and the
	//	parameter object should be initialized.
	// Postconditions:	A boolean is returned.
	virtual bool operator<(const Object& obj) const;
	virtual bool operator>(const Object& obj) const; 
	virtual bool operator<=(const Object& obj) const;
	virtual bool operator>=(const Object& obj) const;
	virtual bool operator==(const Object& obj) const;
	virtual bool operator!=(const Object& obj) const;

};

