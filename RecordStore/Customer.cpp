// Customer.cpp
// Authors:		Andrew VanKooten
// Description:	A class that represents a customer in a database.
// Assumptions:	There will be a person class to derive from.

#include "Customer.h"


Customer::Customer() : Person()
{
}

// Overloaded Constructor
// Description:	Creates a new Customer with the parameters
// Preconditions: None.
// Postconditions: New Customer created with a specified names and id
Customer::Customer(string &first, string &last, int id) : Person(first, last)
{
	ID = id;
}

Customer::~Customer()
{
}

// display()
// Description: Virtual display function that displays the customer ID, the base person, and their history
// Precondition: None
// Postcondition: That customer is displayed on the console
void Customer::display() const
{
	cout << "Transaction History for Customer: " << ID << " ";
	Person::display();
	cout << endl;
	displayHistory();
}

// displayHistory()
// Description:	Displays the entire history.
// Preconditions: None.
// Postconditions: All the Customer's transactions is displayed
void Customer::displayHistory() const
{
	int size = history.size();
	for (int i = 0; i < size; i++)
	{
		cout << history[i] << endl;
	}
}

// Returns the customer ID
int Customer::getID() const { return ID; }
// Sets the customer ID
void Customer::setID(int id) { ID = id; }

// addToHistory()
// Description:	Adds a formatted string, representing a transaction initiated by the Customer,
//	to the history. Stored chronologically
// Preconditions: None.
// Postconditions: string is added to history
void Customer::addHistory(string& hist)
{
	history.push_back(hist);
}

// Comparison Operators
// Description: These operators compare p and *this first by last name then first nae\me
// Preconditions: p points to data on the heap
// Postconditions: a const bool is returned if the condition is met
bool Customer::operator<(const Person &p) const
{
	const Customer& customer = static_cast<const Customer&>(p);
	if (lastName < customer.lastName) return true;
	if (firstName < customer.firstName) return true;
	return false;
}
bool Customer::operator>(const Person &p) const
{
	const Customer& customer = static_cast<const Customer&>(p);
	if (lastName > customer.lastName) return true;
	if (firstName > customer.firstName) return true;
	return false;
}
bool Customer::operator<=(const Person &p) const
{
	const Customer& customer = static_cast<const Customer&>(p);
	if (lastName <= customer.lastName) return true;
	if (firstName <= customer.firstName) return true;
	return false;
}
bool Customer::operator>=(const Person &p) const
{
	const Customer& customer = static_cast<const Customer&>(p);
	if (lastName >= customer.lastName) return true;
	if (firstName >= customer.firstName) return true;
	return false;
}
bool Customer::operator==(const Person &p) const
{
	const Customer& customer = static_cast<const Customer&>(p);
	if (ID == customer.ID && firstName == customer.firstName && lastName == customer.lastName) return true;
	else return false;
}
bool Customer::operator!=(const Person &p) const
{
	return !(*this == p);
}