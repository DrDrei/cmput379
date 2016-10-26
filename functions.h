#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE; 

typedef struct Handshake {
	BYTE FIRST;
	BYTE SECOND;
};

void sendUserMessage(int sock, char * buffer, char * username) {
	bzero(buffer, sizeof(buffer));
	fgets(buffer, sizeof(buffer)-1, stdin);
	send(sock, buffer, sizeof(buffer)-1, 0);
}

void userUpdate(int type) {

}
