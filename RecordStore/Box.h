// Box.h
// Author: Andrew VanKooten
// Description: Abstract class that makes it so inventory objects and customer objects can be stored in BSTree
// Assumptions: None
#pragma once

#include <fstream>
using namespace std;

class Box
{
	// operator<<
	// Description: Displays the contents in box
	// Preconditions: box has data initialized
	// Postconditions: virtual display method is called
	friend ostream& operator<<(ostream& OutStream, const Box& box);

public:
	//Default Constructor 
	Box();

	//Default Destructor
	virtual ~Box();

	// display()
	// Description: pure virtual display method
	// Preconditions: none
	// Postconditions: none
	virtual void display() const = 0;

	// Comparison Operators
	// Description: Compares the box data with *this data
	// Preconditions: box is initialized and has data
	// Postconditions: a bool is returned 
	virtual bool operator<(const Box& box) const = 0;
	virtual bool operator==(const Box& box) const = 0;
	virtual bool operator!=(const Box& box) const = 0;

	// getters
	// getHashableValue()
	// returns an int holding hashablevalue
	int getHashableValue() const;

	// getType()
	// returns a char representing the type:
	char getType() const;

protected:
	int hashableValue;

	char type;
};

