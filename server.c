

// #define	MY_PORT	2234

#include "sharedFunc.h"

int main(int argc, char *argv[]) {
	int	sock, snew, fromlength, number, outnum;
	struct	sockaddr_in	master, from;
	int portname = 2222;
	
	int messageLength = 0;

	unsigned int userCount = 0;
	unsigned int userCount2 = 0;
	char buffer[256];
	char username[256];
	char updateMessage[256];
	char * handshake[2];
	handshake[0] = (char *) 0xCF;
	handshake[1] = (char *) 0xA7;

	char * userList[256];

	int * userCountPtr = &userCount;
	char ** usernameArray = malloc(userCount);
	// timming to cut connection

	/*
	*	Do we want to use a signal handler to stop the child 
	*	process when connection is closed?
	*
	*/
    struct timeval tv = {30, 0};
    struct timeval tv2 = {5, 0};
    fd_set readfds, readfds2;
	//FD_ZERO(&readfds);

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
	int fd[2], fd2[2]; // file descriptors
	int parentVal = 0;
	pipe(fd);
	int checkSel2;

	while (1) {
		fromlength = sizeof (from);
		while(1) {
			struct timeval tv2 = {5, 0}; // reset timer
			FD_SET(sock, &readfds2);
	        checkSel2 = select(sock+1, &readfds2, NULL, NULL, &tv2);
			
			if( checkSel2 < 0) {
				perror("select");// error
			} else if (FD_ISSET(sock, &readfds2)) {

				snew = accept(sock, (struct sockaddr*) &from, &fromlength);

				break;
	        } else {
	        	// check if there is an update message
	        	//read(fd[0], &updateMessage, sizeof(updateMessage));
	        	//printf("Time out %s\n", updateMessage+2);
	            
	         }

		}
		
		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}

		
		memset(username, sizeof(username), 0);
		// send the handshake
		send(snew, &handshake, sizeof(handshake)-1, 0); 
		// recive the first byte
		recv(snew, messageLength, 1, 0); // frecieve the username
		recv(snew, buffer, messageLength, 0); // frecieve the username
		


		userList[userCount] = (username+1);
		size_t i;
		for(i = 0; i < userCount; ++i) {
			printf("%s\n", userList[i]);	
		}


// 		for(size_t i = 0; i < userCount; ++i) {
// 			printf("%s\n", usernameArray[i]);	
// 		}

	    pid = fork();
		buffer[0] = 0x01; // new user has joined

	    if ( pid  == -1) { // failure
			close(snew);
			continue;
		} else if (pid > 0) {

			// this is the parent (main) server
			close(snew); // we dont need the main server to be connected
			
			// this is for user joining
			memcpy(&updateMessage[1], username, sizeof(updateMessage));
			write(fd[1], &updateMessage, sizeof(updateMessage));
		} else if (pid == 0) {
			printf("I am a CHILD server with pid %d\n", getpid());
			printf("hi\n");

			read(fd[0], &updateMessage, sizeof(updateMessage));	

			printf("Parent has sent a message %s\n", updateMessage+2);

		}


		/*
		* in the while loop for the child process
		*
		*/
		int connected = 1;
		int recvCheck, checkSel;
		printf("User count is:%d\n", userCount);

		while(!pid) {

			struct timeval tv = {20, 0}; // reset timer
			FD_SET(snew, &readfds);
			checkSel = select(snew+1, &readfds, NULL, NULL, &tv);
			if( checkSel < 0) {
				perror("select");

			} else if (FD_ISSET(snew, &readfds)) {
				
				memset(buffer, 0, 255);
				recvCheck = recv(snew, buffer, sizeof(buffer), 0);

				// keep this??? if we ctrl-c it will keep printing 0 otherwise
				if(buffer[0] == 0) { continue; }

				//printf("recvCheck is %d\n", recvCheck);
				// testing	
				printf("Server with pid %d Recieved Message: \n", getpid());
				// there is a god dammed return carriage here. im thinking we 
				// user buffer[0] to find it and set that byte to something else
				memset(buffer+ (int)buffer[0] , 0, 1); // sets the byte after the message to 0
				printf("%s  -- Length: %d\n", buffer+1, (int) buffer[0]); // buffer+1 ignores first byte
				
				//write(fd2[1], &buffer, sizeof(buffer)); // write client message to pipe

			} else {
				printf("I have exited\n");
				fflush(0);

				memset(updateMessage + (int)buffer[0], 0, 1);
				//memset(updateMessage, 2, 1);
				write(fd2[1], &buffer, sizeof(buffer)); // write client message to pipe

				//printf("Update Message closing: %d, %s\n", updateMessage[0], updateMessage+2);
				close(snew);
				exit(0);
			}

			
		}

		close (snew);
	}
}







