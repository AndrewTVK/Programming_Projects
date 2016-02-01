// TransactionFactory.h
// Authors : Andrew VanKooten
// Description : A class that creates objects that are a subtype of the Transaction class, and returns them as a pointer
//	to the Transaction class. Used with the Factory Method Pattern.
// Assumptions : Anytime a new, non - abstract Transaction subclass is written, it's concrete object is stored here. 
#pragma once

#include "Purchase.h"
#include "Trade.h"
#include "History.h"
#include "HistoryAll.h"
#include "Inventory.h"

class TransactionFactory
{
private:
	Transaction* transactions[NUM_LETTERS];

	// hashFunction()
	// returns an int that can be use to locate items in transactions[]
	int hashFunction(char T);
public:
	//Default Constructor
	TransactionFactory();
	//Default Destructor
	~TransactionFactory();

	// createTransaction
	// Description:	Takes a char, from which it determines the desired concrete class and returns it
	//	inside a common Transaction*.
	// Preconditions:	The type parameter must be associated with a non-abstract Transaction subclass, with the
	//	transactions properties properly formatted for that subclass.
	// Postconditions:	A Transaction* is returned containing an object of the desired subtype.
	Transaction* createTransaction(char T, string& descriptions);
};

