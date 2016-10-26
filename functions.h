void sendUserMessage(int sock, char * buffer, char * username) {
	


	bzero(buffer, sizeof(buffer));
	fgets(buffer, sizeof(buffer)-1, stdin);
	send(sock, buffer, sizeof(buffer)-1, 0);
}

void recvUserMessage()

void userUpdate(int type, ) {
	
}