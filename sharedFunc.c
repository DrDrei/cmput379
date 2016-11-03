

void formatMessage(char * message, char * buffer) {

	int messageLength = strlen(message);
	memset(buffer, 0, sizeof(buffer));
	buffer[0] = messageLength;
	memcpy(&buffer[1], message, messageLength+1);	
	
}
