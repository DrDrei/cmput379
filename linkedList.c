


typedef struct Node {

	char *data;
	struct Node *next, *previous;

} Node;


// create a node and initialize its data to str, return a pointer to that node
Node * createNode(char * str) {

	Node *node = malloc(sizeof(Node));
	node->data = malloc(strlen(str)+1);
	memcpy(node->data, str, strlen(str)+1);
	node->next = 0;
	node->previous = 0;
	return node;

} 

// deletes and frees memory from a node given a pointer to that node
void deleteNode(Node *node) {
	free(node->data);
	free(node);
}

// append a node with data = str to the tail of the list 
void listAppend(Node *node, char * str) {

	Node *currentNode = node;
	Node *previousNode;
		
	// find our last node
	while(currentNode->next != 0) {
		currentNode = currentNode->next;	
	}

	currentNode->next = createNode(str);
	(currentNode->next)->previous = currentNode;

}

// find the node given a string and return a pointer to the node
// returns 0 if it cannot be found
Node * listFind(Node *node, char *str) {
	
	Node *currentNode = node;
	while(strcmp(str, currentNode->data)) { // has not found it
		if(!currentNode->next) {
			return 0; // cannot find it
		}
		currentNode = currentNode->next;
	}

	// found it
	return currentNode;

}

// remove a node given a str and returns the new head (if it has changed)
Node * listRemove(Node *node, char * str) {

	Node * toRemove = listFind(node, str);
	if(toRemove != 0) { // we found it
		if(toRemove->previous == 0) { // head node is to be removed
			if(toRemove->next == 0) { // the head node is the only node in the list
				free(toRemove);
				return 0;
			}
			
			node = node->next; // set new head node
			(toRemove->next)-> previous = 0;

			free(toRemove);


		} else {
		
			toRemove->previous = toRemove->next; // also accounts for tail nodes
			free(toRemove);
		
		}
	}

	return node; // returns the head

}

// prints out all the nodes
void  listPrint(Node *node) {

	if(node != 0) {

		Node *currentNode = node;

		printf("[ ");
		printf("%s ", currentNode->data);	
		
		// find our last node
		while(currentNode->next != 0) {
			currentNode = currentNode->next;
			printf("%s ", currentNode->data);
			
		}


		printf("]\n");
	} else {
		printf("[ ]\n");
	}


}

// frees the allocated memory from the linked list
void listFree(Node * node) {

	if(node->next == 0) {
		free(node);
	} else {

		Node * currentNode = node->next;

		while(currentNode->next != 0) {	
		
			free(currentNode->previous);
			currentNode = currentNode->next;
		}

		free(currentNode);


	}
	
}