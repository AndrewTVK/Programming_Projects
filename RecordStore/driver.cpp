//driver.cpp
//Author: Andrew VanKooten
//Description: driver for the store class
//Assumptions: appropiate files are included
#include "Store.h"

int main() 
{
	string inventoryList = "hw4inventory.txt", customerList = "hw4customers.txt", commandList = "hw4commands.txt";
	Store RecordStore = Store();

	bool test;
	test = RecordStore.InitializeInventory(inventoryList);
	if (!test) {
		cout << "Cannot Initialize Inventory!" << endl;
		return 1;
	}
	test = RecordStore.InitializeCustomers(customerList);
	if (!test) {
		cout << "Cannot Initialize Customers!" << endl;
		return 1;
	}
	test = RecordStore.InitializeCommands(commandList);
	if (!test) {
		cout << "Cannot Initialize Commands!" << endl;
		return 1;
	}
	RecordStore.ExecuteCommands();


	return 0;
}