
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int formatMessage(char * message, char * buffer) {

	int messageLength = strlen(message);
	printf("My length is %d\n", messageLength);
	memset(buffer, 0, sizeof(buffer));
	buffer[0] = messageLength;
	memcpy(&buffer[1], message, messageLength+1);	
	
	return messageLength;
}