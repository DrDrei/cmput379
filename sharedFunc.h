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

#include <signal.h>

#define STDIN 0 

// formats messages and username from client to server
void formatMessage(char * message, char * buffer);

//void pipeToParent(char *m)