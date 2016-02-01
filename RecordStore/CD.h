//Cd.h
//Authors:		Andrew VanKooten
//Description:	Abstract class that represents a Cd that is sold in a store, that is derived from an object class
//Assumptions:	None.

#pragma once
#include "Object.h"
class CD : public Object
{
protected:
	string artistName;
	int issueYear;
	string title;
public:
	//Default constructor
	CD();
	//Default desstructor
	virtual ~CD();

	// display()
	//Virtual display method that displays the artist, year, and title
	//Preconditions: None
	//Postconditions: The artist, year, and the title will be display on the console
	virtual void display() const;

	// Getters
	//Returns a string representing the artist
	string getArtist();
	//Returns an int representing the year
	int getYear();
	//Returns a string representing the title
	string getTitle();

	
};

