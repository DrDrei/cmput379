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

	number = 0;

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
		
		send(snew, &handshake, sizeof(handshake)-1, 0);

	    pid = fork();
		printf("Return value of fork %d\n", pid);
	    if ( pid  == -1) { // failure ... please never evaluate to true .... please
			close(snew);
			continue;
		} else if (pid > 0) {
			close(snew); // we need this open? well not for my tests but later on...
			++counter;
			printf("I am a PARENT server with pid %d\n", getpid());
			
			parentVal = 100;
			write(fd[1], &parentVal, sizeof(parentVal)); // not through socket but pipe
		} else if (pid == 0) {
			++counter;
			printf("I am a CHILD server with pid %d\n", getpid());
			
			read(fd[0], &parentVal, sizeof(parentVal));
			printf("Parent has sent a message %d\n", parentVal);
		}



		while(!pid) {
			memset(buffer, 256, 0);
			recv(snew, buffer, sizeof(buffer), 0);
		
			printf("Server with pid %d Recieved Message: \n", getpid());
			printf("%s", buffer);
		}

		close (snew);
		number++;
	}
}







