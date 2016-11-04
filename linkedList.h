#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node {

	char *data;
	struct Node *next, *previous;

} Node;
// parameters are the head of list and the string
Node * createNode(char * str); 
void listAppend(Node *node, char * str);
Node * listFind(Node *node, char *str);
Node * listRemove(Node *node, char * str);

void  listPrint(Node *node);
void listFree(Node * node);

#endif