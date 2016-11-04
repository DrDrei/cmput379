#ifndef LINKEDLIST_H
#define LINKEDLIST_H


// A linked list that is used to hold the string of usernames

typedef struct Node {

	char *data; // holds the string for usernames
	struct Node *next, *previous;

} Node;

// any function argument 'Node * node' is the head of 
// the linked list 

// create a node and initialize its data to str, return a pointer to that node
Node * createNode(char * str); 

// append a node with data = str to the tail of the list 
void listAppend(Node *node, char * str);

// find the node given a string and return a pointer to the node
// returns 0 if it cannot be found
Node * listFind(Node *node, char *str);

// remove a node given a str and returns the new head (if it has changed)
Node * listRemove(Node *node, char * str);

// prints the nodes in the list
void  listPrint(Node *node);

// frees the allocated memory from the linked list
void listFree(Node * node);

#endif