//Cd.cpp
//Authors:		Andrew VanKooten
//Description:	Abstract class that represents a Cd that is sold in a store, that is derived from an object class
//Assumptions:	None.
#include "CD.h"

CD::CD()
{
}

CD::~CD()
{
}

// display()
//Virtual display method that displays the artist, year, and title
//Preconditions: None
//Postconditions: The artist, year, and the title will be display on the console
void CD::display() const  
{
	cout << artistName << " " << title << " " << issueYear << " ";
}

//Returns a string representing the artist
string CD::getArtist() { return artistName; }
//Returns a string representing the title
string CD::getTitle() { return title; }
//Returns an int representing the year
int CD::getYear() { return issueYear; }