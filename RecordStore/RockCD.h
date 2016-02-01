// RockCd.h
// Authors : Andrew VanKooten
// Description : Represents a Rock CD that is sold in a store. Derived from CD
// Assumptions : There is CD.h 
#pragma once
#include "CD.h"
class RockCD : public CD
{
public:
	//Default Constructor
	RockCD();
	//Default Destructor
	virtual ~RockCD();

	// Overloaded constructor
	// Accepts a string as input and parses that string and uses that info
	//	to initialize its member functions
	// Preconditions: The string must have the required information in it.
	// Postconditions: *this will be set to the info found in the string
	RockCD(string description);

	// create()
	// Description: This function returns an Object pointer type which points to an
	//	object of a derived class.
	// Preconditions: The description needs to be formatted correctly, determined
	//	by the class that is implementing it.
	// Postconditions: An Object* is returned, pointing to an RockCD object.
	Object* create(string& description);

	// display()
	//Virtual display method that displays the artist, year, and title
	//Preconditions: None
	//Postconditions: The CD display will be called.
	virtual void display() const;

	// Comparison Operators
	// Description: These operators compare RockCd first by artist
	//	name, then by year, and then by title.
	// Preconditions: The comparable data members of both the calling object and the
	//	parameter object should be initialized.
	// Postconditions: A boolean is returned.
	bool RockCD::operator<(const Object& obj) const;
	bool RockCD::operator>(const Object& obj) const;
	bool RockCD::operator<=(const Object& obj) const;
	bool RockCD::operator>=(const Object& obj) const;
	bool RockCD::operator==(const Object& obj) const;
	bool RockCD::operator!=(const Object& obj) const;

};

