

// #define	MY_PORT	2234

#include "sharedFunc.h"
#include "linkedList.h"
#include "include.h"

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

	//int * userCountPtr = &userCount;
	//char ** usernameArray = malloc(userCount);

	Node *userListHead;

	// timming to cut connection

	/*
	*	Do we want to use a signal handler to stop the child 
	*	process when connection is closed?
	*
	*/
    struct timeval tv = {30, 0};
    struct timeval tv2 = {5, 0};
    struct timeval tv3 = {1, 0};
    fd_set readfds, readfds2, readfds3;
	FD_ZERO(&readfds);
	FD_ZERO(&readfds2);
	FD_ZERO(&readfds3);



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
	int checkSel2, checkSel3;

	int fdList[10][2];

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
	        	int j;
	        	for(j = 0; j < userCount; ++j){

	        	 	struct timeval tv3 = {1,0};
	        	 	checkSel3 = select(fd[j]+1, &readfds3, NULL, NULL, &tv3);
	        	 	if(checkSel3 < 0){
	        	 		perror("select3");
	        	 	} else if (checkSel3 == 0) {

	        	 		read(fdList[j][0], &updateMessage, 1);
	        	 		read(fdList[j][0], &updateMessage+1, updateMessage[0]);
	        	 		printf("Index: %d\n", j);
	        	 		printf("Time out %s\n", updateMessage+1);
	        	 	} else { // no activity
	        	 		printf("No activity\n");
	        	 		break;
	        	 	}
	        	// 	// check for removals here
	        		

	        	// 	// check if there is an update message
	        	// 	//read(fd[0], &updateMessage, sizeof(updateMessage));
	        		
	            }
	         }

		}
		
		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}

		
		memset(username, sizeof(username), 0);
		
		send(snew, &handshake, sizeof(handshake)-1, 0); // send the handshake
		
										 // we have to change this, it should take a buffer
		recv(snew, username, 1, 0); // recieve the first two bytes 
		messageLength = (int)username[0];
		//printf("Length of recieved thing is %d \n", messageLength);
		fflush(0);
		recv(snew, username+1, messageLength, 0); // recieve the username
		printf("My recieved usename is: %s\n", username+1);
		
		if(userCount == 0) {
			userListHead = createNode(username+1);
		} else {
			listAppend(userListHead, username+1);
		}
		
		pipe(fdList[userCount]);

		++userCount;
// 		for(size_t i = 0; i < userCount; ++i) {
// 			printf("%s\n", usernameArray[i]);	
// 		}

	    pid = fork();
		buffer[0] = 0x01; // new user has joined

	    if ( pid  == -1) { // failure
			close(snew);
			continue;
		} else if (pid > 0) { // this is the parent (main) server

			close(snew); // we dont need the main server to be connected
			
			// this is for user joining
			memcpy(&updateMessage, username, sizeof(updateMessage));
			//write(fd[1], &updateMessage, sizeof(updateMessage));
			write(fdList[userCount-1][1], &updateMessage, sizeof(updateMessage));
			printf("I want to send message: %s\n", updateMessage+1);

			printf("User count %d\n", userCount);

		} else if (pid == 0) { // child server

			printf("I am a CHILD server with pid %d\n", getpid());
			printf("hi\n");
			printf("User count %d\n", userCount);
			// testing pipe usage
			read(fdList[userCount-1][0], &updateMessage, sizeof(updateMessage));	

			printf("Parent has sent a message %s\n", updateMessage+1);

		}


		/*
		* in the while loop for the child process
		*
		*/
		int connected = 1;
		int recvCheck, checkSel;
		printf("User count is:%d\n", userCount);

		while(!pid) {

			struct timeval tv = {10, 0}; // reset timer
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
				// what is the size??????????//
				printf("Usercount is %d\n", userCount);
				write(fdList[userCount-1][1], &updateMessage, messageLength+1); // write client message to pipe

				//printf("Update Message closing: %d, %s\n", updateMessage[0], updateMessage+2);
				close(snew);
				exit(0);
			}

			
		}

		close (snew);
	}
}







