#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

#include <signal.h>

#define STDIN 0 

// formats messages and username from client to server
void formatMessage(char * message, char * buffer);

//void pipeToParent(char *m)

void arrayAdd(char** array, int *sizePtr, char* user) {
	(*sizePtr)++;
	array = realloc(array, (*sizePtr)*sizeof(char*));
	array[(*sizePtr)-1] = user;
}

void arrayRemove(char** array, int *sizePtr, char* user) {
	int i;
	for (i = 0; i < (*sizePtr); i++) {
		if (array[i] == user) {
			array[i] = array[(*sizePtr)-1];
			(*sizePtr)--;
			array = realloc(array, (*sizePtr)*sizeof(char*));
			break;
		}
	}
}

int arrayCheck(char** array, int *sizePtr, char* user) {
	int i;
	for (i = 0; i < (*sizePtr); i++) {
		if (array[i] == user) {
			return 1;
		}
	}
	return 0;
}