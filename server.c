

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

	//char * userList[256];

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
    struct timeval tv4 = {1, 0};
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
	//pipe(fd);
	int checkSel2, checkSel3, checkSel4;

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
	        	int j, k;
	        	for(j = 0; j < userCount2; ++j){

	        	 	struct timeval tv3 = {1,0};
	        	 	checkSel3 = select(fdList[j][0], &readfds3, NULL, NULL, &tv3);
	        	 	if(checkSel3 < 0){
	        	 		perror("select3");
	        	 	} else if (checkSel3 == 0) { // there are things in the pipe
	        	 		
	        	 		--userCount;
	        	 		memset(updateMessage, 0, sizeof(updateMessage));
	        	 		read(fdList[j][0], &updateMessage, sizeof(updateMessage));
	        	 		
	        	 		//printf("Index before: %d -- ", j);
	        	 		//printf("%s\n", updateMessage+2);
	        	 		userListHead = listRemove(userListHead, updateMessage+2);
	        	 		for(k = 0; k < userCount2; ++j) {
	        	 			write(fdList[k][1], &updateMessage, sizeof(updateMessage));
	        	 		}

	        	 		listPrint(userListHead);
	        	 		//printf("Time out %s\n", updateMessage+2);

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
		//printf("My recieved usename is: %s\n", username+1);
		
		if(userCount == 0) {
			userListHead = createNode(username+1);
			listPrint(userListHead);

		} else {
			listAppend(userListHead, username+1);
			listPrint(userListHead);

		}
		
		pipe(fdList[userCount]);

		++userCount;
		++userCount2;
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
			//memcpy(&updateMessage, username, sizeof(updateMessage));
			//write(fdList[userCount-1][1], &updateMessage, sizeof(updateMessage));

		} else if (pid == 0) { // child server
			// testing pipe usage
			//read(fdList[userCount-1][0], &updateMessage, sizeof(updateMessage));	

		}


		/*
		* in the while loop for the child process
		*
		*/
		int recvCheck, checkSel;

		while(!pid) {

		
	
			struct timeval tv = {7, 0}; // reset timer
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

						
						//memset(updateMessage + (int)buffer[0], 0, 1);
				updateMessage[0] = 0x01; // set first byte to indicate closing connection
				printf("username is.... %s\n", username+1);

						// copy over the username
				memcpy(updateMessage+1, username, sizeof(updateMessage));
						
				write(fdList[userCount-1][1], &updateMessage, sizeof(updateMessage)); // write client message to pipe

				close(snew);
				exit(0);
			}

							

			

			
		}

		close (snew);
	}
}

	// struct timeval tv4 = {2, 0}; // reset timer
	// 		//FD_SET(snew, &readfds);
	// 		checkSel4 = select(fdList[userCount-1][0], &readfds3, NULL, NULL, &tv4);
	// 		if( checkSel4 < 0) {
	// 			perror("select4");

	// 		} else if (checkSel4 == 0) { // there are things in the pipe
	// 			printf("There are things in the pipe\n");
	// 			//printf("Index before: %d -- ", j);

	        	 		
				
	// 		} else { // nothing in the pipe
	// 			printf("Nothing in the pipe\n");
	// 			read(fdList[userCount-1][0], &updateMessage, sizeof(updateMessage));	
			
	// 			printf("Message is a username? %s\n", updateMessage+2);

	// 		}




// struct timeval tv = {7, 0}; // reset timer
// 			FD_SET(snew, &readfds);
// 			checkSel = select(snew+1, &readfds, NULL, NULL, &tv);
// 			if( checkSel < 0) {
// 				perror("select");

// 			} else if (FD_ISSET(snew, &readfds)) {
				
// 				memset(buffer, 0, 255);
// 				recvCheck = recv(snew, buffer, sizeof(buffer), 0);

// 				// keep this??? if we ctrl-c it will keep printing 0 otherwise
// 				if(buffer[0] == 0) { continue; }

// 				//printf("recvCheck is %d\n", recvCheck);
// 				// testing	
// 				printf("Server with pid %d Recieved Message: \n", getpid());
// 				// there is a god dammed return carriage here. im thinking we 
// 				// user buffer[0] to find it and set that byte to something else
// 				memset(buffer+ (int)buffer[0] , 0, 1); // sets the byte after the message to 0
// 				printf("%s  -- Length: %d\n", buffer+1, (int) buffer[0]); // buffer+1 ignores first byte
				
// 				//write(fd2[1], &buffer, sizeof(buffer)); // write client message to pipe

// 			} else {
// 				printf("I have exited\n");
// 				fflush(0);

				
// 				//memset(updateMessage + (int)buffer[0], 0, 1);
// 				updateMessage[0] = 0x01; // set first byte to indicate closing connection
// 				printf("username is.... %s\n", username+1);

// 				// copy over the username
// 				memcpy(updateMessage+1, username, sizeof(updateMessage));
				
// 				write(fdList[userCount-1][1], &updateMessage, sizeof(updateMessage)); // write client message to pipe

// 				close(snew);
// 				exit(0);
// 			}

			
