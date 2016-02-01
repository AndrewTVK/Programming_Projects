// Store.h
// Author : Andrew VanKooten
// Description: Object meant to represent a store with an inventory, customer list, and queue to hold transactions
// Assumptions: Appropiate files are included
#pragma once

#include "DVD.h"
#include "ClassicalCD.h"
#include "RockCD.h"
#include "ObjectFactory.h"
#include "Customer.h"
#include "BSTree.h"
#include "ObjectBox.h"
#include "CustomerBox.h"
#include "HashTable.h"
#include "TransactionFactory.h"

#include <vector>
#include <string>
#include <queue>
using namespace std;

class Store
{
private:
	BSTree* inventory[NUM_LETTERS];	//Holds the inventory
	ObjectFactory objectFactory = ObjectFactory();
	BSTree customers = BSTree(); //Holds customers
	Customer* customerList[1000]; //Holds customer
	queue<Transaction*> commands; //Holds commands
	TransactionFactory transactionFactory = TransactionFactory();

	//inventoryHash()
	//returns an int to figure out where to access a data member in inventory
	int inventoryHash(char C);
public:
	//Default Constructor
	Store();
	//Default Destructor
	~Store();

	// InitializeInventory()
	// Description: creates inventory objects based on input from the file 
	// Preconditions: There is a file with the inventory and the name is passed in through the string
	// Postconditons: If the file is not found false is returned and the program finishes else true is returned
	bool InitializeInventory(string& InventoryList);

	// InitializeCustomers()
	// Description: creates customer objects based on input from the file 
	// Preconditions: There is a file with the customer list and the name is passed in trough the string
	// Postconditions: If the file is not found false is returned and the program finishes  else true is returned
	bool InitializeCustomers(string& CustomerList);

	// InitializeCommands()
	// Description: creates transaction objects based on input from the file
	// Preconditions: There is a file with the commands and the name is passed in through the string
	// Postconditions: If the file is not found false is returned and the program finishes else true is returned
	bool InitializeCommands(string& CommandList);

	// ExecutedCommands()
	// Description: Transactions are possed of the queue and processed
	// Preconditions: None
	// Postconditions: The queue will be empty and transactions in it will be processed
	void ExecuteCommands();
};

