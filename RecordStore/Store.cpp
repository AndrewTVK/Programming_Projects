// Store.cpp
// Author : Andrew VanKooten
// Description: Object meant to represent a store with an inventory, customer list, and queue to hold transactions
// Assumptions: Appropiate files are included
#include "Store.h"

Store::Store()
{
	for (int i = 0; i < NUM_LETTERS; i++)
	{
		inventory[i] = NULL;
	}
}

Store::~Store()
{
	for (int i = 0; i < NUM_LETTERS; i++)
	{
		if (inventory[i] != NULL)
		{
			delete inventory[i];
		}
	}

}

// InitializeInventory()
// Description: creates inventory objects based on input from the file 
// Preconditions: There is a file with the inventory and the name is passed in through the string
// Postconditons: If the file is not found false is returned and the program finishes else true is returned
bool Store::InitializeInventory(string& InventoryList)
{
	Object* object;

	ifstream input(InventoryList);
	if (!input) {
		return false;
	}
	string StringType, stringCount, description;
	for (;;)
	{
		if (input.eof()) break;
		StringType = input.get();				//Get type
		if (StringType[0] == '\n') continue;
		input.get();							//Throw away space
		getline(input, stringCount, ',');		//Get amount
		input.get();							//Throw away space
		getline(input, description);			//Get the rest of the information

		char type = StringType[0];
		int intCount = atoi(stringCount.c_str());

		object = objectFactory.createObject(type, description);
		if (object != NULL) 
		{
			if (inventory[inventoryHash(type)] == NULL)
			{
				inventory[inventoryHash(type)] = new BSTree();
			}
			ObjectBox* container = new ObjectBox(object);
			for (int i = 0; i < intCount; i++)
			{
				inventory[inventoryHash(type)]->insert(container);
			}
		}
	}
	input.close();
	return true;
}

// InitializeCustomers()
// Description: creates customer objects based on input from the file 
// Preconditions: There is a file with the customer list and the name is passed in trough the string
// Postconditions: If the file is not found false is returned and the program finishes  else true is returned
bool Store::InitializeCustomers(string& CustomerList)
{
	Customer *customer;

	ifstream input(CustomerList);
	if (!input) return false;
	string stringID, firstName, lastName;
	for (;;)
	{
		if (input.eof()) break;
		getline(input, stringID, ',');	//Get ID
		input.get();					//Throw away space
		getline(input, lastName, ',');	//Get last name
		input.get();					//Throw away space
		getline(input, firstName);	  	//Get first name

		int ID = atoi(stringID.c_str());

		customer = new Customer(firstName, lastName, ID);
		CustomerBox *newCustomer = new CustomerBox(customer);

		customers.insert(newCustomer); //Store in BSTree
		customerList[ID] = customer; //Store in HashTable
	}
	input.close();
	return true;
}

// InitializeCommands()
// Description: creates transaction objects based on input from the file
// Preconditions: There is a file with the commands and the name is passed in through the string
// Postconditions: If the file is not found false is returned and the program finishes else true is returned
bool Store::InitializeCommands(string& CommandList)
{
	Transaction *command;

	ifstream input(CommandList);
	if (!input) return false;
	string description;
	char type, check;

	for (;;)
	{
		if (input.eof()) break;
		type = input.get();
		check = input.peek();
		if (check == ',')
		{
			input.get(); //Throw away comma
			input.get(); //Throw away space
			getline(input, description);
		}
		else
		{
			input.get();
			description = "";
		}
		command = transactionFactory.createTransaction(type, description);
		if (command != NULL) {
			commands.push(command);
		}
	}
	input.close();
	return true;
}

// ExecutedCommands()
// Description: Transactions are possed of the queue and processed
// Preconditions: None
// Postconditions: The queue will be empty and transactions in it will be processed
void Store::ExecuteCommands()
{
	Transaction* command;
	while (!commands.empty())
	{
		command = commands.front();
		command->executeCommand(inventory, customerList, customers);
		commands.pop();
		delete command;
	}
}

//inventoryHash()
//returns an int to figure out where to access a data member in inventory
int Store::inventoryHash(char C)
{
	return (C - 'A');
}