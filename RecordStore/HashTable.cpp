#include "HashTable.h"


HashTable::HashTable()
{
	for (int i = 0; i < NUM_CUSTOMERS; i++)
	{
		customers[i] = NULL;
	}
}

HashTable::~HashTable()
{

}

void HashTable::insert(CustomerBox* box)
{
	customers[box->getHashableValue()] = box;
}

CustomerBox* HashTable::retreive(int hash)
{
	return customers[hash];
}
