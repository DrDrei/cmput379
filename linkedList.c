


typedef struct Node {

	char *data;
	struct Node *next, *previous;

} Node;

Node * createNode(char * str) {

	Node *node = malloc(sizeof(Node));
	node->data = str;
	node->next = 0;
	node->previous = 0;
	return node;

} 
// parameters are the head of list and the string
void listAppend(Node *node, char * str) {

	Node *currentNode = node;
	Node *previousNode;

	printf("We are appending, there are already things in node:\n");
	printf("%s\n", currentNode->data);
		
	// find our last node
	while(currentNode->next != 0) {
		currentNode = currentNode->next;
		printf("%s\n", currentNode->data);
	
	}

	currentNode->next = createNode(str);
	(currentNode->next)->previous = currentNode;

}

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

Node * listRemove(Node *node, char * str) {

	Node * toRemove = listFind(node, str);
	if(toRemove != 0) { // we found it
		if(toRemove->previous == 0) { // head node is to be removed

			printf("True\n");
			node = node->next; // set new head node
			(toRemove->next)-> previous = 0;
			free(toRemove);


		} else {
		
			toRemove->previous = toRemove->next; // also acounts for tail nodes
			free(toRemove);
		
		}
	}

	return node; // returns the head
}


void  listPrint(Node *node) {

	Node *currentNode = node;

	printf("[ ");
	printf("%s ", currentNode->data);	
	
	// find our last node
	while(currentNode->next != 0) {
		currentNode = currentNode->next;
		printf("%s ", currentNode->data);
		
	}


	printf("]\n");

}

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