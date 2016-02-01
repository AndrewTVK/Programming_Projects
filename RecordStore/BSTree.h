#pragma once
#include "Box.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

//BSTree.h
//Binary Search Tree class
//Author: Andrew VanKooten

//BSTree class: Data structure to hold TreeData items; items are automatically sorted
//	-- includes overloaded assignment operator, output operator, and comparison operators
//	-- includes insert, retrive, remove, and empty functions
//	-- includes depth and descendants functions
//	-- includes recursive helpers for manipulating data
//	-- each node includes a count of duplicates so extra data isn't stored in the tree
//	-- each count will be initialized to 1

// Assumptions:
//	-- there is a TreeData class to hold data
//	-- no datatypes other than the intended one will be tried to be stored in the tree
//	-- output operator will display the data stored in the TreeData followed by the count

class BSTree
{

	//Overloaded Output Operator
	//Calls the DisplayInOrder function with the root address
	//Precondition: Tree must point to data on the heap
	//Postcondition: The contents of Tree are displayed on the console
	friend ostream& operator<<(ostream& OutStream, BSTree& Tree);

private:
	struct Node
	{
		Box *item;			//Holds the TreeData
		Node *left;			//Pointer to the left subtree
		Node *right;		//Pointer to the right subtree
		int count;			//Holds the count if there is more than one of that TreeData
	};
	Node *root;				//Holds the top root

	//CopyTree
	//Recursive helper for the copy constructor and assignment operator
	//Copies the contents of the tree to the current tree
	//Precondition: thisTree and sourceTree point to data stored on the heap
	//Postcondition: A deep copy of sourceTree is now stored in thisTree
	void copyTree(Node *&thisRoot, Node *&sourceRoot);

	//Insert Helper
	//Recursive insert helper
	//Searches the list for the correct position for item and inserts it in the tree
	//Precondition: head points to data on the heap; item points to data on the heap
	//Postcondition: When the correct spot is found a new Node is created and stored in that spot
	bool insert(Node *&head, Box *item);

	//Make Empty Helper
	//Recursive makeEmpty helper; Traverses the subtree and deletes the contents of the node and the node
	//Precondition: head points to data on the heap
	//Postcondition: All nodes stored underneath head will be deleted and head will be equal to NULL 
	void makeEmpty(Node *&head);

	//Output Helper
	//Recursive output operator; Traverses the Tree in order and adds the values and count to the output stream
	//Precondition: head points to data on the heap
	//Postcondition: The data stored in Node is displayed in order
	ostream& DisplayInOrder(ostream& OutStream, Node *&head);

	//Retrieve Helper
	//Recursive retrieve helper; Finds the key node and returns the TreeData pointer
	//Precondition: key points to data on the heap; head points to data on the heap
	//Postcondition: A pointer holding the addess of the TreeData is returned
	const Box *retrieve(const Box& key, Node *&head);

	//Depth Helper
	//Recursive depth helper; Finds the key value and returns the level it is at; If key is not found returns -1
	//Precondition: key points to memory on the heap; head points to data on the heap
	//Postcondition: The node which stores key is found and the depth is returned
	int depth(const Box& key, Node *&head, int depth);

	//Remove Helper
	//Recursive remove helper; Finds the key value and calls the delete function
	//Precondition: key points to data on the heap; heap points to data on the heap
	//Postcondition: the node is found and is deleted and nodes underneath are reattached
	bool remove(const Box& key, Node *&head);

	//DeleteRoot
	//Deletes the current node and reattaches any nodes underneath it
	//Precondition: head points to data on the heap
	//Postcondition: head is deleted and replaced with the smallest value in it's subtree 
	void deleteRoot(Node *&head);

	//FindAndDeleteSmallest
	//Deletes the smallest node in the subtree and returns the value from it
	//Precondition: head points to data on the heap
	//Postcondition: the smallest node in the subtree is found, deleted, and returned
	Box *findAndDeleteSmallest(Node *&head);

	//Descendants Helper
	//Recursive helper for the descendants function
	//Precondition: head points to data on the heap; count is initialized to a value
	//Postcondition: returns the number of nodes underneath the head node
	int descendants(Node *&head, int& count) const;

	//IsEqual
	//Recurisive helper for the equality operator. 
	//Returns false if the sturcture of the tree, data, or count of nodes are different.
	//Precondition: currentHead and otherHead point to data stored in the heap
	//Postcondition: true is returned if the nodes are equal to each other
	bool isEqual(Node *&currentHead, Node *&otherHead);

public:

	//Default Constructor
	//Sets the root to NULL
	//Precondition: None
	//Postcondition: Root points to NULL
	BSTree();

	//Copy Constructor
	//Copies tree to the current tree
	//Precondition: tree.root points to memory on the heap
	//Postcondition: tree is copied to *this
	BSTree(BSTree& tree);

	//Destructor
	//Calls the makeEmpty function
	//Precondition: root points to memory on the heap
	//Postcondition: Data stored in the tree is deallocated and root is set to NULL
	~BSTree();

	//Assignment Operator
	//Copies tree to the current object
	//Precondition: tree.root points to memory on the heap 
	//Postcondition: Data stored in *this is now equal to data stored in tree
	BSTree& operator=(BSTree& tree);

	//Overloaded == Operator
	//Returns true if the current tree and the parameter tree are equal
	//Precondition: tree points to data on the heap
	//Postcondition: returns true if *this and tree are equal to each other
	bool operator==(BSTree& tree);
	
	//Overloaded != Operator
	//Returns true if the current tree and the parameter tree are not equal.
	//Precondition: tree points to data on the heap
	//Postcondition: returns true if *this and tree are not equal
	bool operator!=(BSTree& tree);
	
	//Insert
	//Calls the insert helper with the root node
	//Precondition: root points to data on the heap; item points to data on the heap
	//Postcondition: item is inserted into the tree in the correct place; Return true if successful
	bool insert(Box *item);

	//Make Empty
	//Calls the makeEmpty helper with the root address
	//Precondition: None
	//Postcondition: Data stored in *this will be deleted and root equal to NULL
	void makeEmpty();

	//Retrieve
	//Calls the retrieve helper with the root address
	//Precondition: key points to data on the heap
	//Postcondition: the treeData that key is equal to is returned
	const Box *retrieve(const Box& key);

	//Depth
	//Calls the depth helper with the root address
	//Precondition: key points to data on the heap
	//Postcondition: returns the depth of the key node
	int depth(const Box& key);

	//IsEmpty
	//Returns true if the tree is empty; false if it isn't
	//Precondition: root must point to NULL or a node
	//Postcondition: returns true if the tree is empty false if it isn't
	bool isEmpty() const;

	//Remove 
	//Calls the remove helper with the root address
	//Precondition: key points to data on the heap
	//Postcondition: the node which stores key is removed and all nodes underneath it are reattached
	bool remove(const Box& key);

	//Descendants
	//Returns the number of nodes underneath the node of the TreeData given
	//Returns -1 if key is not found
	//Precondition: key points to data on the heap
	//Postcondition: if key is not found -1 is returned else the descendants of the subtree  is returned
	int descendants(const Box& key) const;
};

