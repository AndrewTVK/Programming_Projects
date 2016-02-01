//History.cpp
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores a HistoryAll transaction
//Assumptions: Appropiate files are included
#include "History.h"


History::History()
{
	type = 'H';
}

History::~History()
{
}

//Overloaded Constructor
//Description: the info in descriptions will be added to the protected members in *this
//Preconditions: descriptions must be correctly formatted
//Postconditions: the members of *this will be equal to the info in descriptions
History::History(string& descriptions)
{
	ID = atoi(descriptions.c_str());
}

//create()
//Description: returns a transaction pointer that points to a history object with the correct data
//Preconditions: descriptions must be formatted correctly
//Postconditions: the history pointer will point to a new history object
Transaction* History::create(string& descriptions)
{
	History* returnPtr = new History(descriptions);
	return returnPtr;
}

//executeCommand
//Description: display the history of the ID customer
//Preconditions: all the accepted objects must point to data on the heap
//Postconditions: the ID customer's history will be displayed
void History::executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers)
{
	Customer* customer = customerList[ID];
	if (customer != NULL)
	{
		cout << *customer << endl;
		cout << endl;
	}
}