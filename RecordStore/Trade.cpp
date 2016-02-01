//Trade.cpp
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores a trade transaction
//Assumptions: Appropiate files are included
#include "Trade.h"

Trade::Trade()
{
}

//Overloaded Constructor
//Description: the info in descriptions will be added to the protected members in *this
//Preconditions: descriptions must be correctly formatted
//Postconditions: the members of *this will be equal to the info in descriptions
Trade::Trade(string& descriptions)
{
	type = 'T';

	stringstream stream(descriptions);
	string stuff;

	//Get customer ID
	getline(stream, stuff, ',');
	stream.get();
	ID = atoi(stuff.c_str());
	stuff = "";

	//Get Object type
	getline(stream, stuff, ',');
	stream.get();
	objectType = stuff[0];
	
	stuff += ", ";

	//Get the rest
	string rest;
	getline(stream, rest);
	history = "Trade: " + stuff + rest;
	item = Factory.createObject(objectType, rest);
}

Trade::~Trade()
{
	delete item;
}

//create()
//Description: returns a transactions pointer that points to a trade object will the correct data
//Preconditions: descriptions must be formatted correctly
//Postconditions: the transaction pointer will point to a new trade object
Transaction* Trade::create(string& descriptions)
{
	Trade* returnPtr = new Trade(descriptions);
	return returnPtr;
}

//executeCommand
//Description: adds item to the Inventory and adds the history to th customer
//Preconditions: all the accepted objects must point to data on the heap
//Postconditions: item will be added to Inventory and the history will be updated
void Trade::executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers)
{
	//If customer doesnt exist, return
	if (customerList[ID] == NULL) return;
	//Add item to Inventory
	ObjectBox* newItem = new ObjectBox(item);
	Inventory[item->getType() - 'A']->insert(newItem);
	//Add item to History
	Customer* customer = customerList[ID];
	if (customer != NULL) customer->addHistory(history);
}