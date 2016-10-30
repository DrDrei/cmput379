#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// #define	MY_PORT	2234

/* ---------------------------------------------------------------------
 This	is  a sample server which opens a stream socket and then awaits
 requests coming from client processes. In response for a request, the
 server sends an integer number  such	 that  different  processes  get
 distinct numbers. The server and the clients may run on different ma-
 chines.
 --------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
	int	sock, snew, fromlength, number, outnum;
	struct	sockaddr_in	master, from;
	int portname = 2222;
	
	unsigned int userCount = 0;
	char buffer[256];
	char username[256];
	char updateMessage[256];
	char * handshake[2];
	handshake[0] = (char *) 0xCF;
	handshake[1] = (char *) 0xA7;

	if (argc == 2) {
		portname = atoi(argv[1]);
   	}

 	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	master.sin_family = AF_INET;
	master.sin_addr.s_addr = INADDR_ANY;
	master.sin_port = htons (portname);

	if (bind (sock, (struct sockaddr*) &master, sizeof(master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

	listen(sock, 5);

	int pid; // used to fork process
	static int counter = 0; // keep count of processes
	int fd[2]; // file descriptor
	int parentVal = 0;
	pipe(fd);

	while (1) {
		fromlength = sizeof (from);
		snew = accept(sock, (struct sockaddr*) &from, &fromlength);
		
		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}

		userCount++;
		
		// send the handshake
		memset(username, sizeof(username), 0);
		send(snew, &handshake, sizeof(handshake)-1, 0); 
		recv(snew, username, sizeof(username)-1, 0); // recieve the username
		printf("%s\n", username);	
	    pid = fork();
		buffer[0] = 0x01; // new user has joined

	    if ( pid  == -1) { // failure
			close(snew);
			continue;
		} else if (pid > 0) {
			// this is the parent (main) server
			close(snew); // we dont need the main server to be connected
			++counter; // why do i have this? can i remove it?
			// this is for user joining

			memcpy(&updateMessage[1], username, sizeof(updateMessage));
			write(fd[1], &updateMessage, sizeof(updateMessage));
		} else if (pid == 0) {
			++counter;
			printf("I am a CHILD server with pid %d\n", getpid());
			
			read(fd[0], &updateMessage, sizeof(updateMessage));
			printf("Parent has sent a message %s\n", updateMessage+1);
		}

		printf("User count is:%d\n", userCount);

		while(!pid) {
			memset(buffer, 0, 255);
			recv(snew, buffer, sizeof(buffer), 0);
	
			// testing	
			printf("Server with pid %d Recieved Message: \n", getpid());
			// there is a god dammed return carriage here. im thinking we 
			// user buffer[0] to find it and set that byte to something else
			memset(buffer+ (int)buffer[0] , 0, 1); // sets the byte after the message to 0
			printf("%s  -- Length: %d\n", buffer+1, (int) buffer[0]); // buffer+1 ignores first byte
			
		}

		close (snew);
	}
}







