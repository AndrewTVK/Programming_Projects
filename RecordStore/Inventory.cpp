//History.cpp
//Author: Andrew VanKooten
//Description: Derived class that derives from Transaction that stores an Inventory transaction
//Assumptions: Appropiate files are included
#include "Inventory.h"

Inventory::Inventory()
{
	type = 'I';
}

Inventory::~Inventory()
{
}

//create()
//Description: returns a transaction pointer that points to a Inventory object with the correct data
//Preconditions: descriptions must be formatted correctly
//Postconditions: the history pointer will point to a new Inventory object
Transaction* Inventory::create(string& descriptions)
{
	Inventory* returnPtr = new Inventory();
	return returnPtr;
}

//executeCommand
//Description: display the inventory
//Preconditions: all the accepted objects must point to data on the heap
//Postconditions: All inventory is displayed
void Inventory::executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers)
{
	BSTree* container;
	cout << "Total Inventory:" << endl;
	for (int i = 0; i < NUM_LETTERS; i++)
	{
		container = Inventory[i];
		if (container != NULL)
		{
			char t = 'A' + i;
			cout << "Displaying objects of type: " << t << endl;
			cout << *container << endl;
		}
	}

}