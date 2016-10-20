


void message(int snew, char* buffer, int length) {
	send(snew, &buffer, sizeof(handshake)-1, 0);
}