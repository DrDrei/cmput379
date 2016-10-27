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

int main(int argc, char *argv[]) {
	int	s, number;
	struct	sockaddr_in	server;
	struct	hostent		*host;

	char buffer[256];
	char * recieved[2];
	char * username = "usernameUndefined";
	int portname = 2222;
	
    struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 3;
    tv.tv_usec = 500000;


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

	
	recv(s, &recieved, sizeof(recieved), 0);
	// handshake
	if (recieved[0] == (char *) 0xCF && recieved[1] == (char *) 0xA7) {
		fprintf (stderr, "Your Process ID: %d \n", getpid());
		
	}

	// Connection Established, Messaging here.
	while (1) { // only for child processes
		FD_ZERO(&readfds);
        FD_SET(STDIN, &readfds);
        select(STDIN+1, &readfds, NULL, NULL, &tv);
        
        if (FD_ISSET(STDIN, &readfds)) {
			printf("Enter your message: ");
			bzero(buffer, 256);
			fgets(buffer, 255, stdin);
			fflush(stdout);
			send(s, buffer, sizeof(buffer)-1, 0);
        } else {
            printf("Timed out.\n");
        }
	}
	close (s);	

}







