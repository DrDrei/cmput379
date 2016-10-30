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

#define STDIN 0  // file descriptor for standard input
// #define	 MY_PORT  2234

/* ---------------------------------------------------------------------
 This is a sample client program for the number server. The client and
 the server need not run on the same machine.				 
 --------------------------------------------------------------------- */

void formatMessage(char * message, char * buffer); 

int main(int argc, char *argv[]) {
	int	s, number;
	struct	sockaddr_in	server;
	struct	hostent		*host;

	char buffer[256];
	char message[256];
	char * recieved[2];
	char * username = "usernameUndefined";
	int messageLength = 0;
	int portname = 2222;
	
    struct timeval tv = {5, 0};
    fd_set readfds;
	FD_ZERO(&readfds);

	host = gethostbyname ("localhost");
	if (argc == 3) {
		portname = atoi(argv[1]);
      	username = argv[2];
   	}

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}

	s = socket (AF_INET, SOCK_STREAM, 0);

	if (s < 0) {
		perror ("Client: cannot open socket");
		exit (1);
	}

	memset(&server, sizeof (server), 0);
	bcopy(host->h_addr, &server.sin_addr, host->h_length);
	server.sin_family = host->h_addrtype;
	server.sin_port = htons (portname);

	if (connect (s, (struct sockaddr*) & server, sizeof (server))) {
		perror ("Client: cannot connect to server");
		exit (1);
	}

	//handshake	
	recv(s, &recieved, sizeof(recieved), 0);
	if (recieved[0] == (char *) 0xCF && recieved[1] == (char *) 0xA7) {
		fprintf (stderr, "Your Process ID: %d \n", getpid());
		formatMessage(username, buffer); 
		printf("Username length is: %d -- buffer: %s\n", messageLength, buffer+1);	
		// sends the length followed by the username
		send(s, buffer, sizeof(buffer)-1, 0);

	}
	int checkSel;
	// Connection Established, Messaging here.
	while (1) { // only for child processes
        struct timeval tv = {5, 0}; // reset timer
		FD_SET(STDIN, &readfds);
        checkSel = select(STDIN+1, &readfds, NULL, NULL, &tv);
		
		if( checkSel == -1) {
			perror("select");// error
		} else if (FD_ISSET(STDIN, &readfds)) {
			// we input our message here
			// should we reset and initialize here as well?
			memset(message, 0, sizeof(message));
			fgets(message, 255, stdin);

			formatMessage(message, buffer);
			//printf("My message \n %s", buffer+1); // +1 ignores the first byte (pointer arithmetic)
			send(s, buffer, sizeof(buffer)-1, 0);
	
        } else {
            printf("Timed out.\n"); // every five seconds
        }

	}
	close (s);	

}

// formats messages and username from client to server
void formatMessage(char * message, char * buffer) {
	
	int messageLength = strlen(message);
	memset(buffer, 0, sizeof(buffer));
	buffer[0] = messageLength;
	memcpy(&buffer[1], message, messageLength+1);	
	
}





