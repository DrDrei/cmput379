
// might want to move this to the header file
 // file descriptor for standard input
// #define	 MY_PORT  2234

#include "sharedFunc.h"
#include "include.h"


void intHandler(int sig);

// global for the socket, used to closed connection on ctrl-c
int s;

int main(int argc, char *argv[]) {
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

	//signal
	signal(SIGINT, intHandler);

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
		messageLength = formatMessage(username, buffer); 
		printf("Username length is: %d -- buffer: %s\n", messageLength, buffer+1);	
		// sends the length followed by the username
		send(s, buffer, messageLength+1, 0);

	}
	int checkSel;
	// Connection Established, Messaging here.
	while (1) { // only for child processes
        struct timeval tv = {5, 0}; // reset timer
		FD_SET(STDIN, &readfds);
        checkSel = select(STDIN+1, &readfds, NULL, NULL, &tv);
		
		if( checkSel < 0) {
			perror("select");// error
		} else if (FD_ISSET(STDIN, &readfds)) {
			// we input our message here
			// should we reset and initialize here as well?
			memset(message, 0, sizeof(message));
			fgets(message, 255, stdin);
			messageLength = formatMessage(message, buffer);
			message[strcspn(message, "\n")] = '\0';

			if (!strncmp(message, "/", 1)) {
				if (!strcmp(message, "/help")) {
					printf("Available functions:\n");
					printf("/list - get a list of current users connected\n");
				} else if (!strcmp(message, "/list")) {
					// provide a list of users
				} else {
					printf("Please type in /help to see a list of functions\n");
				}
			} else {
				//printf("My message \n %s", buffer+1); // +1 ignores the first byte (pointer arithmetic)
				send(s, buffer, message+1, 0);
			}
	
        } else {
            printf("Timed out.\n"); // every five seconds
        }
	}
	close (s);	
}


void intHandler(int sig) {
	char c;
	signal(sig, SIG_IGN);
	printf("You pressed ctrl-c?\n");
   	close(s);
    exit(0);
}

