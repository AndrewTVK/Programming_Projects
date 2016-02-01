// Person.h
// Authors : Andrew VanKooten
// Description : An abstract class that represents a person in a database.
// Assumptions : None.
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

class Person
{
	// operator<<
	// Description:	Sends the Person to the output stream and the virtual display is called
	// Preconditions:	p is initialized
	// Postconditions:	The Person is sent to the output stream.
	friend ostream& operator<<(ostream& OutStream, const Person &p);

protected:
	string firstName;
	string lastName;

public:
	//Default Constructor
	Person();
	Person(string &first, string &last);
	// Default Destructor
	virtual ~Person();

	//Getters
	//Returns a string that is the person's first name
	string getFirst() const;
	//Returns a string that is the person's last name
	string getLast() const;

	//Setters
	//sets a person's firstName to first
	void setFirst(string &first);
	//sets a person's lastName to last
	void setLast(string &last);

	// display()
	// Virtual method that displays the last and first name
	// Preconditions: None
	// Postconditions: lastName and firstName are displayed on the console
	virtual void display() const;

	// Comparison Operators
	// Description: These operators compare lastName first then by firstName
	// Preconditions: The comparable data members of both the calling object and the
	//	parameter object should be initialized.
	// Postconditions: A boolean is returned.
	virtual bool operator<(const Person &p) const = 0;
	virtual bool operator>(const Person &p) const = 0;
	virtual bool operator<=(const Person &p) const = 0;
	virtual bool operator>=(const Person &p) const = 0;
	virtual bool operator==(const Person &p) const = 0;
	virtual bool operator!=(const Person &p) const = 0;
};

