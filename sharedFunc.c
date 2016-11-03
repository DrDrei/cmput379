

int formatMessage(char * message, char * buffer) {

	int messageLength = strlen(message);
	printf("My length is %d\n", messageLength);
	memset(buffer, 0, sizeof(buffer));
	buffer[0] = messageLength;
	memcpy(&buffer[1], message, messageLength+1);	
	
	return messageLength;
}

void arrayAdd(char** array, int *sizePtr, char* user) {
	printf("Usercount is: %d\n",*sizePtr);
	(*sizePtr)++;
	printf("Usercount is: %d\n",*sizePtr);
	char ** temp = array;
	array = realloc(temp, (*sizePtr)*sizeof(char*));
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