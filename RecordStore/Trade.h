//Trade.h
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores a trade transaction
//Assumptions: Appropiate files are included

#pragma once
#include "Transaction.h"

class Trade : public Transaction
{
protected:
	int ID;
	char objectType;
	Object* item;

public:
	//Default Constructor
	Trade();

	//Overloaded Constructor
	//Description: the info in descriptions will be added to the protected members in *this
	//Preconditions: descriptions must be correctly formatted
	//Postconditions: the members of *this will be equal to the info in descriptions
	Trade(string& descriptions);

	//Default Destructor
	virtual ~Trade();

	//create()
	//Description: returns a transactions pointer that points to a trade object will the correct data
	//Preconditions: descriptions must be formatted correctly
	//Postconditions: the transaction pointer will point to a new trade object
	Transaction* create(string& descriptions);

	//executeCommand
	//Description: adds item to the Inventory and adds the history to th customer
	//Preconditions: all the accepted objects must point to data on the heap
	//Postconditions: item will be added to Inventory and the history will be updated
	virtual void executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers);
};

