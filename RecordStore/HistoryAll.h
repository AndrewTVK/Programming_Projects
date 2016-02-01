//HistoryAll.h
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores a HistoryAll transaction
//Assumptions: Appropiate files are included
#pragma once
#include "Transaction.h"
class HistoryAll : public Transaction
{
public:
	//Default Constructor
	HistoryAll();

	//Default Destructor
	virtual ~HistoryAll();

	//create()
	//Description: returns a transaction pointer that points to a HistoryAll object with the correct data
	//Preconditions: descriptions must be formatted correctly
	//Postconditions: the history pointer will point to a new HistoryAll object
	Transaction* create(string& descriptions);

	//executeCommand
	//Description: display the history for all customers
	//Preconditions: all the accepted objects must point to data on the heap
	//Postconditions: All customer histories will be displayed
	virtual void executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers);
};

