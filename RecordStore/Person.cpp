// Person.cpp
// Authors : Andrew VanKooten
// Description : An abstract class that represents a person in a database.
// Assumptions : None.
#include "Person.h"

Person::Person()
{
	firstName = "";
	lastName = "";
}

Person::~Person()
{
}

Person::Person(string &first, string &last)
{
	firstName = first;
	lastName = last;
}

//Returns a string that is the person's first name
string Person::getFirst() const { return firstName; }
//Returns a string that is the person's last name
string Person::getLast() const { return lastName; }

//sets a person's firstName to first
void Person::setFirst(string &first) { firstName = first; }
//sets a person's lastName to last
void Person::setLast(string &last) { lastName = last; }

// display()
// Virtual method that displays the last and first name
// Preconditions: None
// Postconditions: lastName and firstName are displayed on the console
void Person::display() const
{
	cout << firstName << " " << lastName << " ";
}

// operator<<
// Description:	Sends the Person to the output stream and the virtual display is called
// Preconditions:	p is initialized
// Postconditions:	The Person is sent to the output stream.
ostream& operator<<(ostream& OutStream, const Person& p)
{
	p.display();
	return OutStream;
}
