//Purchase.cpp
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores a purchase transaction
//Assumptions: Appropiate files are included
#include "Purchase.h"

Purchase::Purchase()
{
	type = 'P';
}

Purchase::~Purchase()
{

}

//Overloaded Constructor
//Description: the info in descriptions will be added to the protected members in *this
//Preconditions: descriptions must be correctly formatted
//Postconditions: the members of *this will be equal to the info in descriptions
Purchase::Purchase(string& descriptions)
{
	type = 'P';
	

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
	history = "Purchase: " + stuff + rest;
	item = Factory.createObject(objectType, rest);

}

//create()
//Description: returns a transactions pointer that points to a purchase object with the correct data
//Preconditions: descriptions must be formatted correctly
//Postconditions: the transaction pointer will point to a new purchase object
Transaction* Purchase::create(string& descriptions)
{
	Purchase *tempPtr = new Purchase(descriptions);
	return tempPtr;
}

//executeCommand
//Description: subtracts item from the Inventory and adds the history to the customer
//Preconditions: all the accepted objects must point to data on the heap
//Postconditions: item will be subtracted from Inventory and the history will be updated
void Purchase::executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers)
{
	if (item == NULL) return;
	ObjectBox removeItem = ObjectBox(item);
	if (Inventory[item->getType() - 'A']->remove(removeItem))
	{
		Customer* customer = customerList[ID];
		if (customer != NULL) customer->addHistory(history);
	}

}