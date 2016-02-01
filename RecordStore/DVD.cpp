// DVD.cpp
// Author Andrew VanKooten
// Description : Represents a Dvd that is sold in a store. Derived from an Object class
// Assumptions : There will be a Object.h class 
#include "DVD.h"

DVD::DVD()
{
	type = 'D';
}

DVD::~DVD() 
{
}

// display()
//Virtual display method that displays the director, year, and title
//Preconditions: None
//Postconditions: The director, title, and yeat will displayed on the console 
void DVD::display() const 
{
	cout << director << " " << title << " " << issueYear << " ";
}


string DVD::getDirector() { return director; }
string DVD::getTitle() { return title; }
int DVD::getYear() { return issueYear; }

// Overloaded constructor
// Accepts a string as input and parses that string and uses that info
//	to initialize its member functions
// Preconditions: The string must have the required information in it.
// Postconditions: *this will be set to the info found in the string
DVD::DVD(string description)
{
	type = 'D';

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
	title = names[1];
	director = names[0];
}

// create()
// Description: This function returns an Object pointer type which points to an
//	object of a derived class.
// Preconditions: The description needs to be formatted correctly, determined
//	by the class that is implementing it.
// Postconditions: An Object* is returned, pointing to an DVD object.
Object* DVD::create(string& description)
{
	Object* returnPtr = new DVD(description);
	return returnPtr;
}

// Comparison Operators
// Description:	These operators compare Dvds first by title, then by year, and then
//	 by director
// Preconditions:	The comparable data members of both the calling object and the
//	parameter object should be initialized.
// Postconditions:	A boolean is returned.
bool DVD::operator<(const Object& obj) const
{
	const DVD& dvd = static_cast<const DVD&>(obj);
	if (title < dvd.title) return true;
	if (issueYear < dvd.issueYear) return true;
	if (director < dvd.director) return true;
	return false;
}
bool DVD::operator>(const Object& obj) const
{
	const DVD& dvd = static_cast<const DVD&>(obj);
	if (title > dvd.title) return true;
	if (issueYear > dvd.issueYear) return true;
	if (director > dvd.director) return true;
	return false;
}
bool DVD::operator<=(const Object& obj) const
{
	const DVD& dvd = static_cast<const DVD&>(obj);
	if (title <= dvd.title) return true;
	if (issueYear <= dvd.issueYear) return true;
	if (director <= dvd.director) return true;
	return false;
}
bool DVD::operator>=(const Object& obj) const
{
	const DVD& dvd = static_cast<const DVD&>(obj);
	if (title >= dvd.title) return true;
	if (issueYear >= dvd.issueYear) return true;
	if (director >= dvd.director) return true;
	return false;
}
bool DVD::operator==(const Object& obj) const
{
	const DVD& dvd = static_cast<const DVD&>(obj);
	if (title == dvd.title && issueYear == dvd.issueYear && director == dvd.director) return true;
	else return false;
}
bool DVD::operator!=(const Object& obj) const
{
	return !(*this == obj);
}