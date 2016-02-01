//Transaction.h
//Author: Andrew VanKooten
//Description: Abstract class to store a type of transaction
//Assumptions: Appropiate files are included
#pragma once

#include "ObjectFactory.h"
#include "RockCD.h"
#include "DVD.h"
#include "ClassicalCD.h"
#include "HashTable.h"
#include "Customer.h"
#include "CustomerBox.h"
#include "ObjectBox.h"
#include "BSTree.h"

#include <string>
#include <sstream>
#include <vector>
using namespace std;
class Transaction
{
protected:
	char type;
	string history;
	ObjectFactory Factory = ObjectFactory();
public:
	//Default Constructor
	Transaction();

	//executeCommand()
	//Description: Pure virtual function to derived classes to execute the command
	//Preconditions: None.
	//Postconditions: None.
	virtual void executeCommand(BSTree* Inventory[], Customer* customerList[], BSTree& customers) = 0;

	//create()
	//Description: Pure virtual function for derived classes to create a specific object
	//Preconditions: None.
	//Postconditions: None.
	virtual Transaction* create(string& description) = 0;

	//Default Destructor
	virtual ~Transaction();
};

