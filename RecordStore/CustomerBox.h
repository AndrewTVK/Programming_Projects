// CustomerBox.h
//	Authors:		Andrew VanKooten
//	Description:	Is derived from a Box class and holds a pointer to a customer object 
//	Assumptions:	There will be a Customer.h and box.h file

#pragma once
#include "Box.h"
#include "Customer.h"
class CustomerBox : public Box
{
public:
	// Default Constructor
	CustomerBox();

	// Overloaded Constructor
	// Description:			Stores the customer pointer from the parameter in data
	// Preconditions:		The accepted customer must point to data on the heap
	// Postconditions:		customer will be equal to the parameter
	CustomerBox(Customer*);

	// Default Destructor
	virtual ~CustomerBox();

	// getCustomer
	// Description:			Returns a pointer to the customer refernce
	// Preconditions:		data must point to customer on the heap
	// Postconditions:		a pointer pointing to customer will be returned
	Customer* getCustomer(int ID);

	// Comparison Operators
	// Description: These operators compares box and customer and 
	//	returns a bool based on if that operation is true or not
	// Preconditons: box must point to data on the heap
	// Postconditions: A bool is returned if the operation is true or not
	virtual bool operator<(const Box& box) const;
	virtual bool operator==(const Box& box) const;
	virtual bool operator!=(const Box& box) const;

	// display()
	// displays the customer 
	// Preconditons: none
	// Postconditions: customer is displayed on the console
	virtual void display() const;

protected:
	Customer* customer = NULL;

	
};

