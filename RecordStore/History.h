//History.h
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores a history transaction
//Assumptions: Appropiate files are included
#pragma once

#include "Transaction.h"
class History : public Transaction 
{
protected:
	int ID;
public:

	//Default Constructor
	History();

	//Overloaded Constructor
	//Description: the info in descriptions will be added to the protected members in *this
	//Preconditions: descriptions must be correctly formatted
	//Postconditions: the members of *this will be equal to the info in descriptions
	History(string& descriptions);

	//Default Destructor
	virtual ~History();

	//create()
	//Description: returns a transaction pointer that points to a history object with the correct data
	//Preconditions: descriptions must be formatted correctly
	//Postconditions: the history pointer will point to a new history object
	Transaction* create(string& descriptions);

	//executeCommand
	//Description: display the history of the ID customer
	//Preconditions: all the accepted objects must point to data on the heap
	//Postconditions: the ID customer's history will be displayed
	virtual void executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers);
};

