


void sendUserMessage(int sock, char* buffer) {
	


	bzero(buffer, sizeof(buffer));
	fgets(buffer, sizeof(buffer)-1, stdin);
	send(sock, buffer, sizeof(buffer)-1, 0);
}

void recvUserMessage()