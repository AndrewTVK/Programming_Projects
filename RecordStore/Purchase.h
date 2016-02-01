//Purchase.h
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores a purchase transaction
//Assumptions: Appropiate files are included
#pragma once
#include "Transaction.h"

class Purchase : public Transaction
{
protected:
	int ID;
	char objectType;
	Object* item;

public:
	//Default Constructor
	Purchase();

	//Overloaded Constructor
	//Description: the info in descriptions will be added to the protected members in *this
	//Preconditions: descriptions must be correctly formatted
	//Postconditions: the members of *this will be equal to the info in descriptions
	Purchase(string& descriptions);
	
	//Default Destructor
	virtual ~Purchase();

	//create()
	//Description: returns a transactions pointer that points to a purchase object with the correct data
	//Preconditions: descriptions must be formatted correctly
	//Postconditions: the transaction pointer will point to a new purchase object
	Transaction* create(string& descriptions);

	//executeCommand
	//Description: subtracts item from the Inventory and adds the history to the customer
	//Preconditions: all the accepted objects must point to data on the heap
	//Postconditions: item will be subtracted from Inventory and the history will be updated
	virtual void executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers);
};

