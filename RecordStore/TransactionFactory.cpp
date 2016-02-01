// TransactionFactory.cpp
// Authors : Andrew VanKooten
// Description : A class that creates objects that are a subtype of the Transaction class, and returns them as a pointer
//	to the Transaction class. Used with the Factory Method Pattern.
// Assumptions : Anytime a new, non - abstract Transaction subclass is written, it's concrete object is stored here. 
#include "TransactionFactory.h"

TransactionFactory::TransactionFactory()
{ 
	for (int i = 0; i < NUM_LETTERS; i++)
	{
		transactions[i] = NULL;
	}
	transactions[hashFunction('P')] = new Purchase();
	transactions[hashFunction('T')] = new Trade();
	transactions[hashFunction('H')] = new History();
	transactions[hashFunction('A')] = new HistoryAll();
	transactions[hashFunction('I')] = new Inventory();
}

TransactionFactory::~TransactionFactory()
{
	for (int i = 0; i < NUM_LETTERS; i++)
	{
		if (transactions[i] != NULL)
		{
			delete transactions[i];
		}
	}
}

// hashFunction()
// returns an int that can be use to locate items in transactions[]
int TransactionFactory::hashFunction(char T)
{
	return (T - 'A');
}

// createTransaction
// Description:	Takes a char, from which it determines the desired concrete class and returns it
//	inside a common Transaction*.
// Preconditions:	The type parameter must be associated with a non-abstract Transaction subclass, with the
//	transactions properties properly formatted for that subclass.
// Postconditions:	A Transaction* is returned containing an object of the desired subtype.
Transaction* TransactionFactory::createTransaction(char T, string& descriptions)
{
	if (T == 'A' || T == 'I' || T == 'T' || T == 'P' || T == 'H')
		return transactions[hashFunction(T)]->create(descriptions);
	else return NULL;
}