//ClassicalCd.cpp
// Authors:		Andrew VanKooten
// Description:	Represents a Classical CD that is sold in a store, that is derived from the CD class.
// Assumptions:	There will be a CD class to derive from

#include "ClassicalCD.h"


ClassicalCD::ClassicalCD()
{
	type = 'C';
}

// Overloaded constructor
// Accepts a string as input and parses that string and uses that info
//	to initialize its member functions
// Preconditions: The string must have the required information in it.
// Postconditions: *this will be set to the info found in the string
ClassicalCD::ClassicalCD(string description)
{
	type = 'C';

	vector<string> names;
	stringstream stream(description);
	while (stream.good())
	{
		string name;
		getline(stream, name, ',');
		names.push_back(name);
		stream.get();
	}
	composer = names[0];
	issueYear = atoi(names[2].c_str());
	artistName = names[3];
	title = names[1];
}

ClassicalCD::~ClassicalCD()
{
}

// display()
//Virtual display method that displays the artist, year, title, and composer
//Preconditions: None
//Postconditions: The CD display will be called and then the composer will be displayed 
void ClassicalCD::display() const
{
	CD::display();
	cout << composer << " ";
}

// Returns a string that represents the composer
string ClassicalCD::getComposer() { return composer; }

// create()
// Description: This function returns an Object pointer type which points to an
//	object of a derived class.
// Preconditions: The description needs to be formatted correctly, determined
//	by the class that is implementing it.
// Postconditions: An Object* is returned, pointing to an ClassicalCd object.
Object* ClassicalCD::create(string& description)
{
	Object* returnPtr = new ClassicalCD(description);
	return returnPtr;
}

// Comparison Operators
// Description: These operators compare ClassicalCd first by composer, then by artist
//	name, then by year, and then by title.
// Preconditions: The comparable data members of both the calling object and the
//	parameter object should be initialized.
// Postconditions: A boolean is returned.
bool ClassicalCD::operator<(const Object& obj) const
{
	const ClassicalCD& cd = static_cast<const ClassicalCD&>(obj);
	if (composer < cd.composer) return true;
	if (artistName < cd.artistName) return true;
	if (issueYear < cd.issueYear) return true;
	if (title < cd.title) return true;
	return false;
}
bool ClassicalCD::operator>(const Object& obj) const
{
	const ClassicalCD& cd = static_cast<const ClassicalCD&>(obj);
	if (composer > cd.composer) return true;
	if (artistName > cd.artistName) return true;
	if (issueYear > cd.issueYear) return true;
	if (title > cd.title) return true;
	return false;
}
bool ClassicalCD::operator<=(const Object& obj) const
{
	const ClassicalCD& cd = static_cast<const ClassicalCD&>(obj);
	if (composer <= cd.composer) return true;
	if (artistName <= cd.artistName) return true;
	if (issueYear <= cd.issueYear) return true;
	if (title <= cd.title) return true;
	return false;
}
bool ClassicalCD::operator>=(const Object& obj) const
{
	const ClassicalCD& cd = static_cast<const ClassicalCD&>(obj);
	if (composer >= cd.composer) return true;
	if (artistName >= cd.artistName) return true;
	if (issueYear >= cd.issueYear) return true;
	if (title >= cd.title) return true;
	return false;
}
bool ClassicalCD::operator==(const Object& obj) const
{
	const ClassicalCD& cd = static_cast<const ClassicalCD&>(obj);
	if (composer == cd.composer && artistName == cd.artistName && issueYear == cd.issueYear && title == cd.title) 
		return true;
	else return false;
}
bool ClassicalCD::operator!=(const Object& obj) const
{
	return !(*this == obj);
}