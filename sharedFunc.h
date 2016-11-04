#ifndef SHAREDFUNC_H
#define SHAREDFUNC_H

// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/time.h>
// #include <sys/types.h>

// #include <signal.h>

#define STDIN 0 


// typedef struct Node {

// 	char *data;
// 	struct Node *next, *previous;

// } Node;

// formats messages and username from client to server
int formatMessage(char * message, char * buffer);

//void pipeToParent(char *m)

void arrayAdd(char** array, int *sizePtr, char* user);

void arrayRemove(char** array, int *sizePtr, char* user);

int arrayCheck(char** array, int *sizePtr, char* user);


// // parameters are the head of list and the string
// Node * createNode(char * str); 
// void listAppend(Node *node, char * str);
// Node * listFind(Node *node, char *str);
// Node * listRemove(Node *node, char * str);
// void  listPrint(Node *node);
// void listFree(Node * node);


#endif