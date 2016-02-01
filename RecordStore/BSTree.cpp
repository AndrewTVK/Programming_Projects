#include "BSTree.h"

//BSTree.cpp
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

//Default Constructor
//Sets the root to NULL
//Precondition: None
//Postcondition: Root points to NULL
BSTree::BSTree()
{
	root = NULL;
}

//Copy Constructor
//Copies tree to the current tree
//Precondition: tree.root points to memory on the heap
//Postcondition: tree is copied to *this
BSTree::BSTree(BSTree& tree)
{
	copyTree(root, tree.root);
}

//Destructor
//Calls the makeEmpty function
//Precondition: root points to memory on the heap
//Postcondition: Data stored in the tree is deallocated and root is set to NULL
BSTree::~BSTree() 
{ 
	makeEmpty(root); 
}

//Assignment Operator
//Copies tree to the current object
//Precondition: tree.root points to memory on the heap 
//Postcondition: Data stored in *this is now equal to data stored in tree
BSTree& BSTree::operator=(BSTree& tree) 
{ 
	if (this != &tree)
	{
		copyTree(root, tree.root);
	}
	return *this;
}

//CopyTree
//Recursive helper for the copy constructor and assignment operator
//Copies the contents of the tree to the current tree
//Precondition: thisTree and sourceTree point to data stored on the heap
//Postcondition: A deep copy of sourceTree is now stored in thisTree
void BSTree::copyTree(Node *&thisTree, Node *&sourceTree)
{
	if (sourceTree == NULL)
		thisTree = NULL;
	else
	{
		thisTree = new Node;
		thisTree->item = sourceTree->item;
		thisTree->count = sourceTree->count;
		copyTree(thisTree->left, sourceTree->left);
		copyTree(thisTree->right, sourceTree->right);
	}
}

//Insert
//Calls the insert helper with the root node
//Precondition: root points to data on the heap; item points to data on the heap
//Postcondition: item is inserted into the tree in the correct place; Return true if successful
bool BSTree::insert(Box *item) 
{ 
	return insert(root, item); 
}

//Insert Helper
//Recursive insert helper
//Searches the list for the correct position for item and inserts it in the tree
//Precondition: head points to data on the heap; item points to data on the heap
//Postcondition: When the correct spot is found a new Node is created and stored in that spot
bool BSTree::insert(Node *&head, Box *item)
{
	//If the correct place is found, create a new node and insert it
	if (head == NULL)
	{
		head = new Node;
		head->item = item;
		head->count = 1;
		head->left = NULL;
		head->right = NULL;
		return true;
	}
	//If the key is already in the tree, increase the count
	else if (*head->item == *item)
	{
		head->count++;
		return false;
	}
	//Search the left if key is smaller
	else if (*item < *head->item) return insert(head->left, item);
	//Search the right
	else return insert(head->right, item);
}

//Make Empty
//Calls the makeEmpty helper with the root address
//Precondition: None
//Postcondition: Data stored in *this will be deleted and root equal to NULL
void BSTree::makeEmpty() 
{ 
	makeEmpty(root); 
}

//Make Empty Helper
//Recursive makeEmpty helper; Traverses the subtree and deletes the contents of the node and the node
//Precondition: head points to data on the heap
//Postcondition: All nodes stored underneath head will be deleted and head will be equal to NULL 
void BSTree::makeEmpty(Node *&head)
{	
	//If head is not equal to NULL
	if (head != NULL)
	{
		//If the left pointer is not equal to NULL search the left tree
		if (head->left != NULL) makeEmpty(head->left);
		//If the right pointer is not equal to NULL search the right tree
		if (head->right != NULL) makeEmpty(head->right);

		//Delete item and the node and set head to NULL
		delete head->item;
		delete head;
		head = NULL;
	}
}

//Overloaded Output Operator
//Calls the DisplayInOrder function with the root address
//Precondition: Tree must point to data on the heap
//Postcondition: The contents of Tree are displayed on the console
ostream& operator<<(ostream& OutStream, BSTree& Tree)
{
	return Tree.DisplayInOrder(OutStream, Tree.root);
}

//Output Helper
//Recursive output operator; Traverses the Tree in order and adds the values and count to the output stream
//Precondition: head points to data on the heap
//Postcondition: The data stored in Node is displayed in order
ostream& BSTree::DisplayInOrder(ostream& OutStream, Node *&head)
{
	//If head is not equal to NULL 
	if (head != NULL)
	{
		//Display the contents of the left tree
		DisplayInOrder(OutStream, head->left);
		//Display the current Node
		OutStream << *head->item;
		if (head->item->getType() == 'O')
		{
			OutStream << ": " << head->count << endl;
		}
		else OutStream << endl;
		//Display the contents of the right tree
		DisplayInOrder(OutStream, head->right);
	}
	return OutStream;
}

//Retrieve
//Calls the retrieve helper with the root address
//Precondition: key points to data on the heap
//Postcondition: the treeData that key is equal to is returned
const Box* BSTree::retrieve(const Box& key) 
{ 
	return retrieve(key, root); 
}

//Retrieve Helper
//Recursive retrieve helper; Finds the key node and returns the TreeData pointer
//Precondition: key points to data on the heap; head points to data on the heap
//Postcondition: A pointer holding the addess of the TreeData is returned
const Box* BSTree::retrieve(const Box& key, Node *&head)
{
	//If key is not found
	if (head == NULL) return NULL;
	//If key is found return it.
	else if (key == *head->item) return head->item;
	//Search the left
	else if (key < *head->item) return retrieve(key, head->left);
	//Search the right
	else return retrieve(key, head->right);
}

//Depth
//Calls the depth helper with the root address
//Precondition: key points to data on the heap
//Postcondition: returns the depth of the key node
int BSTree::depth(const Box& key)
{
	return depth(key, root, 0);
}

//Depth Helper
//Recursive depth helper; Finds the key value and returns the level it is at; If key is not found returns -1
//Precondition: key points to memory on the heap; head points to data on the heap
//Postcondition: The node which stores key is found and the depth is returned
int BSTree::depth(const Box& key, Node *&head, int level)
{
	//If key is not found return -1
	if (head == NULL) return -1;
	//If key is found return the level
	else if (key == *head->item) return level;
	//Search the left
	else if (key < *head->item) return depth(key, head->left, level + 1);
	//Search the right
	else return depth(key, head->right, level + 1);
}

//Remove 
//Calls the remove helper with the root address
//Precondition: key points to data on the heap
//Postcondition: the node which stores key is removed and all nodes underneath it are reattached
bool BSTree::remove(const Box& key) 
{ 
	return remove(key, root); 
}

//Remove Helper
//Recursive remove helper; Finds the key value and calls the delete function
//Precondition: key points to data on the heap; heap points to data on the heap
//Postcondition: the node is found and is deleted and nodes underneath are reattached
bool BSTree::remove(const Box& key, Node *&head)
{
	//If key is not found return false
	if (head == NULL) return false;
	//If key is found delete node and return true
	else if (key == *head->item)
	{
		//If the count is more than 1 decrease the count
		if (head->count > 1) head->count--;
		else return false;
 		return true;
	}
	//Search the left
	else if (key < *root->item) return remove(key, head->left);
	//Search the right
	else return remove(key, head->right);
}

//DeleteRoot
//Deletes the current node and reattaches any nodes underneath it
//Precondition: head points to data on the heap
//Postcondition: head is deleted and replaced with the smallest value in it's subtree 
void BSTree::deleteRoot(Node *&head)
{
	//If head is a leaf delete it
	if (head->left == NULL && head->right == NULL)
	{
		delete head->item;
		delete head;
		head = NULL;
	}
	//If left is equal to NULL attach the right tree and delete the node
	else if (head->left == NULL)
	{
		Node *tempNode = head;
		head = head->right;
		delete tempNode->item;
		delete tempNode;
	}
	//If right is equal to NULL attach the left tree and delete the node
	else if (head->right == NULL)
	{
		Node *tempNode = head;
		head = head->left;
		delete tempNode->item;
		delete tempNode;
	}
	//If left and right are attached to nodes delete the item and replace it with the smallest node in the subtree
	else
	{
		delete head->item;
		head->item = findAndDeleteSmallest(head->right);
	}
}

//FindAndDeleteSmallest
//Deletes the smallest node in the subtree and returns the value from it
//Precondition: head points to data on the heap
//Postcondition: the smallest node in the subtree is found, deleted, and returned
Box* BSTree::findAndDeleteSmallest(Node *&head)
{
	//If the smallest one is found 
	if (head->left == NULL)
	{
		Box *item = head->item;
		Node *temp = head;
		head = head->right;
		delete temp;
		return item;
	}
	//Search the left
	else return findAndDeleteSmallest(head->left);
}

//Descendants
//Returns the number of nodes underneath the node of the TreeData given
//Returns -1 if key is not found
//Precondition: key points to data on the heap
//Postcondition: if key is not found -1 is returned else the descendants of the subtree  is returned
int BSTree::descendants(const Box& key) const
{
	Node *head = root;
	int children = 0;
	//Find the key in the tree
	for (;;)
	{
		if (head == NULL) return -1;
		else if (key == *head->item) return descendants(head, children);
		else if (key < *head->item) head = head->left;
		else head = head->right;
	}
}

//Descendants Helper
//Recursive helper for the descendants function
//Precondition: head points to data on the heap; count is initialized to a value
//Postcondition: returns the number of nodes underneath the head node
int BSTree::descendants(Node *&head, int& count) const
{
	//If the left subtree is not equal to NULL
	if (head->left != NULL)
	{
		count++;
		descendants(head->left, count);
	}
	//If the right subtree is not qual to NULL
	if (head->right != NULL)
	{
		count++; 
		descendants(head->right, count);
	}
	return count;
}

//Overloaded == Operator
//Returns true if the current tree and the parameter tree are equal
//Precondition: tree points to data on the heap
//Postcondition: returns true if *this and tree are equal to each other
bool BSTree::operator==(BSTree& tree)
{
	return isEqual(root, tree.root);
}

//Overloaded != Operator
//Returns true if the current tree and the parameter tree are not equal.
//Precondition: tree points to data on the heap
//Postcondition: returns true if *this and tree are not equal
bool BSTree::operator!=(BSTree& tree) 
{ 
	return !(*this == tree);
}

//IsEqual
//Recurisive helper for the equality operator. 
//Returns false if the sturcture of the tree, data, or count of nodes are different.
//Precondition: currentHead and otherHead point to data stored in the heap
//Postcondition: true is returned if the nodes are equal to each other
bool BSTree::isEqual(Node *&currentHead, Node *&otherHead)
{
	if (currentHead != NULL && otherHead != NULL)
	{
		//If the current item and the source item are not equal return false
		if (*currentHead->item != *otherHead->item) return false;
		//If the current item count is not equal to the source item count return false
		if (currentHead->count != otherHead->count) return false;
		return isEqual(currentHead->left, otherHead->left) && isEqual(currentHead->right, otherHead->right);
	}
	else if (currentHead == NULL && otherHead == NULL) return true;
	else return false;
}

//IsEmpty
//Returns true if the tree is empty; false if it isn't
//Precondition: root must point to NULL or a node
//Postcondition: returns true if the tree is empty false if it isn't
bool BSTree::isEmpty() const 
{ 
	return root == NULL; 
}