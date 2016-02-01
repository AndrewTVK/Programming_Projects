// RockCd.cpp
// Authors : Andrew VanKooten
// Description : Represents a Rock CD that is sold in a store. Derived from CD
// Assumptions : There is CD.h 
#include "RockCD.h"

RockCD::RockCD()
{
}

RockCD::~RockCD()
{
}

// Overloaded constructor
// Accepts a string as input and parses that string and uses that info
//	to initialize its member functions
// Preconditions: The string must have the required information in it.
// Postconditions: *this will be set to the info found in the string
RockCD::RockCD(string description)
{
	type = 'R';

	vector<string> names;
	stringstream stream(description);
	while (stream.good())
	{
		string name;
		getline(stream, name, ',');
		names.push_back(name);
		stream.get();
	}
	issueYear = atoi(names[2].c_str());
	artistName = names[0];
	title = names[1];
}

// create()
// Description: This function returns an Object pointer type which points to an
//	object of a derived class.
// Preconditions: The description needs to be formatted correctly, determined
//	by the class that is implementing it.
// Postconditions: An Object* is returned, pointing to an RockCD object.
Object* RockCD::create(string& description)
{
	Object* returnPtr = new RockCD(description);
	return returnPtr;
}

// display()
//Virtual display method that displays the artist, year, and title
//Preconditions: None
//Postconditions: The CD display will be called.
void RockCD::display() const 
{
	CD::display();
}

// Comparison Operators
// Description: These operators compare RockCd first by artist
//	name, then by year, and then by title.
// Preconditions: The comparable data members of both the calling object and the
//	parameter object should be initialized.
// Postconditions: A boolean is returned.
bool RockCD::operator<(const Object& obj) const
{
	const RockCD& cd = static_cast<const RockCD&>(obj);
	if (artistName < cd.artistName) return true;
	if (issueYear < cd.issueYear) return true;
	if (title < cd.title) return true;
	return false;
}
bool RockCD::operator<=(const Object& obj) const
{
	const RockCD& cd = static_cast<const RockCD&>(obj);
	if (artistName > cd.artistName) return true;
	if (issueYear > cd.issueYear) return true;
	if (title > cd.title) return true;
	return false;
}
bool RockCD::operator>(const Object& obj) const
{
	const RockCD& cd = static_cast<const RockCD&>(obj);
	if (artistName <= cd.artistName) return true;
	if (issueYear <= cd.issueYear) return true;
	if (title <= cd.title) return true;
	return false;
}
bool RockCD::operator>=(const Object& obj) const
{
	const RockCD& cd = static_cast<const RockCD&>(obj);
	if (artistName >= cd.artistName) return true;
	if (issueYear >= cd.issueYear) return true;
	if (title >= cd.title) return true;
	return false;
}
bool RockCD::operator==(const Object& obj) const
{
	const RockCD& cd = static_cast<const RockCD&>(obj);
	if (artistName == cd.artistName && issueYear == cd.issueYear && title == cd.title) return true;
	else return false;
}
bool RockCD::operator!=(const Object& obj) const
{
	return !(*this == obj);
}