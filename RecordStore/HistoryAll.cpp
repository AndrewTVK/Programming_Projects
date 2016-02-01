//HistoryAll.cpp
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores a HistoryAll transaction
//Assumptions: Appropiate files are included
#include "HistoryAll.h"

HistoryAll::HistoryAll()
{
	type = 'A';
}

HistoryAll::~HistoryAll()
{
}

//create()
//Description: returns a transaction pointer that points to a HistoryAll object with the correct data
//Preconditions: descriptions must be formatted correctly
//Postconditions: the history pointer will point to a new HistoryAll object
Transaction* HistoryAll::create(string& descriptions)
{
	HistoryAll* returnPtr = new HistoryAll();
	return returnPtr;
}

//executeCommand
//Description: display the history for all customers
//Preconditions: all the accepted objects must point to data on the heap
//Postconditions: All customer histories will be displayed
void HistoryAll::executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers)
{
	cout << "Transaction History For All Customers: " << endl;
	cout << customers << endl;
}