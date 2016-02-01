//History.h
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores an Inventory transaction
//Assumptions: Appropiate files are included

#pragma once
#include "Transaction.h"
class Inventory : public Transaction
{
public:
	//Default Constructor
	Inventory();
	//Default Destructor
	virtual ~Inventory();

	//create()
	//Description: returns a transaction pointer that points to a Inventory object with the correct data
	//Preconditions: descriptions must be formatted correctly
	//Postconditions: the history pointer will point to a new Inventory object
	Transaction* create(string& descriptions);

	//executeCommand
	//Description: display the inventory
	//Preconditions: all the accepted objects must point to data on the heap
	//Postconditions: All inventory is displayed
	virtual void executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers);
};

