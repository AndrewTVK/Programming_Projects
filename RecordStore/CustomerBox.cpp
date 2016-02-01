// CustomerBox.cpp
//	Authors:		Andrew VanKooten
//	Description:	Is derived from a Box class and holds a pointer to a customer object 
//	Assumptions:	There will be a Customer.h and box.h file

#include "CustomerBox.h"


CustomerBox::CustomerBox()
{

}

// Overloaded Constructor
// Description:			Stores the customer pointer from the parameter in data
// Preconditions:		The accepted customer must point to data on the heap
// Postconditions:		customer will be equal to the parameter
CustomerBox::CustomerBox(Customer* cust)
{
	customer = cust;
	hashableValue = cust->getID();
	type = 'C';
}

CustomerBox::~CustomerBox()
{
	delete customer;
}

// getCustomer
// Description:			Returns a pointer to the customer refernce
// Preconditions:		data must point to customer on the heap
// Postconditions:		a pointer pointing to customer will be returned
Customer* CustomerBox::getCustomer(int ID)
{
	return customer;
}

// display()
// displays the customer 
// Preconditons: none
// Postconditions: customer is displayed on the console
void CustomerBox::display() const
{
	cout << *customer;
}

// Comparison Operators
// Description: These operators compares box and customer and 
//	returns a bool based on if that operation is true or not
// Preconditons: box must point to data on the heap
// Postconditions: A bool is returned if the operation is true or not
bool CustomerBox::operator<(const Box& box) const
{
	const CustomerBox& cust = static_cast<const CustomerBox&>(box);
	if (*customer < *cust.customer) return true;
	else return false;
}
bool CustomerBox::operator==(const Box& box) const
{
	const CustomerBox& cust = static_cast<const CustomerBox&>(box);
	if (*customer == *cust.customer) return true;
	else return false;
}
bool CustomerBox::operator!=(const Box& box) const
{
	return !(*this == box);
}