#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "linkedList.h"

int main() {

	Node * head = createNode("pies");
	char username[256];

	listPrint(head);
	memset(username,0, sizeof(username));
	username[0] = 'a';
	username[1] = 'b';
	username[2] = 'c';
	username[3] = 'd';
	username[4] = 'e';
	username[5] = 'f';
	username[6] = 'g';
	username[7] = 'h';
	username[8] = 0;
	//fgets(username, 255, stdin);
	listAppend(head, username);

	memset(username,0, sizeof(username));
	//fgets(username, 255, stdin);
	listAppend(head, username);

	memset(username,0, sizeof(username));
	//fgets(username, 255, stdin);
	listAppend(head, username);

	listPrint(head);

	return 0;
}
