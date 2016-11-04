

// #define	MY_PORT	2234

#include "sharedFunc.h"
#include "linkedList.h"
#include "include.h"

void intHandler(int sig);
int sock;
FILE *filePtr;

int main(int argc, char *argv[]) {
	Node *userListHead;

	int	snew, fromlength;
	struct	sockaddr_in	master, from;
	int portname = 2222;
	
	int messageLength = 0;

	// =====why are we using two userCounts????
	unsigned int userCount = 0;
	unsigned int userCount2 = 0;

	char buffer[256];
	char username[256];
	char updateMessage[256];
	char * handshake[2];
	handshake[0] = (char *) 0xCF;
	handshake[1] = (char *) 0xA7;

    fd_set readfds, readfds2, readfds3, readfds4;

	FD_ZERO(&readfds);
	FD_ZERO(&readfds2);
	FD_ZERO(&readfds3);
	FD_ZERO(&readfds4);

	signal(SIGINT, intHandler);


	char filename[50]; 
	sprintf(filename, "server379%i.log", getpid());
	filePtr = fopen(filename, "rb+");
	if (filePtr == NULL) {
		filePtr = fopen(filename, "wb");
	}


	if (argc == 2) { // check for command line arg of portnumber
		portname = atoi(argv[1]);
   	}
   	
 	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	master.sin_family = AF_INET;
	master.sin_addr.s_addr = INADDR_ANY;
	master.sin_port = htons(portname);
	if (bind (sock, (struct sockaddr*) &master, sizeof(master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

	listen(sock, 5);

	//pipe(fd);
	int checkSel2, checkSel3;

	int fdList[10][2];

	while (1) {
		fromlength = sizeof(from);
		while(1) {

			// check if there is new client connections
			struct timeval tv2 = {5, 0}; // reset timer
			FD_SET(sock, &readfds2);
	        checkSel2 = select(sock+1, &readfds2, NULL, NULL, &tv2);
			
			if( checkSel2 < 0) { // error
				perror("select error with connection to client");
			} else if (FD_ISSET(sock, &readfds2)) {	
				// client connection
				snew = accept(sock, (struct sockaddr*) &from, &fromlength);
				break; // move on so we can for the server

	        } else {

	        	int j, k; // iterate through the array of pipes from parent to child servers
	        	for(j = 0; j < userCount2; ++j){

	        		// check for update messages
	        	 	struct timeval tv3 = {2,0}; // reset timer to check for update messages
	        	 	checkSel3 = select(fdList[j][0], &readfds3, NULL, NULL, &tv3);
	        	 	if(checkSel3 < 0){
	        	 		perror("select3");
	        	 	} else if (checkSel3 == 0) { // there is an update message
	        	 		
	        	 		if(userCount == 0) {
	        	 			continue;
	        	 		}
	        	 		--userCount;
	        	 		memset(updateMessage, 0, sizeof(updateMessage));
	        	 		read(fdList[j][0], &updateMessage, sizeof(updateMessage));
	        	 		userListHead = listRemove(userListHead, updateMessage+2);

	        	 		// so i need to work on this part. it segfaults here
	        	 		// pipe the update message to all child processes
	        	 		// FOUND OUT WHY
	        	 		// NO SHIT WE SEGFAULT HERE, THE CHILD SERVERS DONT READ FROM IT
	        	 		for(k = 0; k < userCount2; ++k) {
	        	 			printf("I want to write %s\n", updateMessage+2);
	        	 		 	write(fdList[k][1], &updateMessage, sizeof(updateMessage));
	        	 		}
	        	 		
	        	 	} else { // no activity
	        	 		break; // keep waiting on more connections
	        	 	}
	            }
	         }
		}
		
		// connection error
		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}

		
		memset(username, sizeof(username), 0); // to store the username and its length
		send(snew, &handshake, sizeof(handshake)-1, 0); // send the handshake
		recv(snew, username, 1, 0); // recieve the length of the username 
		messageLength = (int)username[0]; // the length is the first byte
		recv(snew, username+1, messageLength, 0); // recieve the username 
		
		// store the usernames in a linked list
		if(userCount == 0) {
			userListHead = createNode(username+1);
		} else {
			if(listFind(userListHead,username+1)) {
				// we found it
				fprintf(filePtr, "User \"%s\" already exits in the list.\n", username+1);
			} else {
				fprintf(filePtr, "User \"%s\" added to list.\n", username+1);
				listAppend(userListHead, username+1);
			}
		}
		
		// open pipe for child server
		pipe(fdList[userCount2]);

		++userCount;
		++userCount2;

		buffer[0] = 0x01; // new user has joined

		// fork the main server

		int pid; // used to fork process
	    pid = fork();
		
	    if ( pid  == -1) { // failure
			close(snew);
			continue;
		} else if (pid > 0) { // this is the parent (main) server
			close(snew); // main server has no reason to still be connected to client
		} 


		int recvCheck, checkSel;

		while(!pid) { // child server
	
			struct timeval tv = {30, 0}; // reset timer
			FD_SET(snew, &readfds);
			FD_SET(fdList[userCount2-1][0], &readfds);

			checkSel = select(snew+1, &readfds, NULL, NULL, &tv);	

			if( checkSel < 0) {
				perror("select");

			} else if (FD_ISSET(snew, &readfds)) {
						
				memset(buffer, 0, 255);
				recvCheck = recv(snew, buffer, sizeof(buffer), 0);

				if(buffer[0] == 0) { continue; }
				printf("Server with pid %d Recieved Message: \n", getpid());
				memset(buffer+ (int)buffer[0] , 0, 1); // sets the byte after the message to 0
				printf("%s  -- Length: %d\n", buffer+1, (int) buffer[0]); // buffer+1 ignores first byte 
			} else {
				fflush(0);
				updateMessage[0] = 0x01; // set first byte to indicate closing connection
				memset(updateMessage, 0, 255);
				memcpy(updateMessage+1, username, sizeof(updateMessage));
				write(fdList[userCount2-1][1], &updateMessage, sizeof(updateMessage)); // write client message to pipe
				close(snew);
				exit(0);
			}
		} // end of child server while loop
	}
}


void intHandler(int sig) {
	fprintf(filePtr, "Terminating...\n");
	signal(sig, SIG_IGN);
   	close(sock);
    exit(0);
}

			
