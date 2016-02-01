//ClassicalCd.h
// Authors:		Andrew VanKooten
// Description:	Represents a Classical CD that is sold in a store, that is derived from the CD class.
// Assumptions:	There will be a CD class to derive from

#pragma once
#include "CD.h"
class ClassicalCD : public CD
{
protected:
	string composer;
public:
	// Default constructor
	ClassicalCD();

	// Overloaded constructor
	// Accepts a string as input and parses that string and uses that info
	//	to initialize its member functions
	// Preconditions: The string must have the required information in it.
	// Postconditions: *this will be set to the info found in the string
	ClassicalCD(string description);

	// Default destructor
	virtual ~ClassicalCD();
	
	// create()
	// Description: This function returns an Object pointer type which points to an
	//	object of a derived class.
	// Preconditions: The description needs to be formatted correctly, determined
	//	by the class that is implementing it.
	// Postconditions: An Object* is returned, pointing to an ClassicalCd object.
	Object* create(string& description);

	// display()
	//Virtual display method that displays the artist, year, title, and composer
	//Preconditions: None
	//Postconditions: The CD display will be called and then the composer will be displayed 
	virtual void display() const;

	// Getters
	// Returns a string that represents the composer
	string getComposer();

	// Comparison Operators
	// Description: These operators compare ClassicalCd first by composer, then by artist
	//	name, then by year, and then by title.
	// Preconditions: The comparable data members of both the calling object and the
	//	parameter object should be initialized.
	// Postconditions: A boolean is returned.
	bool operator<(const Object& obj) const;
	bool operator>(const Object& obj) const;
	bool operator<=(const Object& obj) const;
	bool operator>=(const Object& obj) const;
	bool operator==(const Object& obj) const;
	bool operator!=(const Object& obj) const;
};

