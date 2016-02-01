// Customer.h
// Authors:		Andrew VanKooten
// Description:	A class that represents a customer in a database.
// Assumptions:	There will be a person class to derive from.

#pragma once
#include "Person.h"
class Customer : public Person
{
protected:
	int ID;

	vector<string> history;
public:
	// Default Constructor
	Customer();

	// Overloaded Constructor
	// Description:	Creates a new Customer with the parameters
	// Preconditions: None.
	// Postconditions: New Customer created with a specified names and id
	Customer(string& first, string& last, int id);
	
	//Default Destructor
	virtual ~Customer();

	// display()
	// Description: Virtual display function that displays the customer ID, the base person, and their history
	// Precondition: None
	// Postcondition: That customer is displayed on the console
	virtual void display() const;

	// displayHistory()
	// Description:	Displays the entire history.
	// Preconditions: None.
	// Postconditions: All the Customer's transactions is displayed
	virtual void displayHistory() const;

	// Getter
	// Returns the customer ID
	int getID() const;

	// Setter
	// Sets the customer ID
	void setID(int id);

	// addToHistory()
	// Description:	Adds a formatted string, representing a transaction initiated by the Customer,
	//	to the history. Stored chronologically
	// Preconditions: None.
	// Postconditions: string is added to history
	void addHistory(string& hist);

	// Comparison Operators
	// Description: These operators compare p and *this first by last name then first nae\me
	// Preconditions: p points to data on the heap
	// Postconditions: a const bool is returned if the condition is met
	virtual bool operator<(const Person &p) const;
	virtual bool operator>(const Person &p) const;
	virtual bool operator<=(const Person &p) const;
	virtual bool operator>=(const Person &p) const;
	virtual bool operator==(const Person &p) const;
	virtual bool operator!=(const Person &p) const;
};

