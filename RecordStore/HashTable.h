#pragma once

#include "CustomerBox.h"

const int NUM_CUSTOMERS = 1000;

class HashTable
{
public:
	HashTable();
	~HashTable();

	void insert(CustomerBox* box);
	CustomerBox* retreive(int hash);
private:
	CustomerBox* customers[NUM_CUSTOMERS];

};

