CC = gcc

# remove if not needed
CFLAGS =

DEPS =

all: client379 server379

client379: client.o sharedFunc.o
	$(CC) $(CFLAGS) client.o sharedFunc.o -o client379

server379: server.o sharedFunc.o
	$(CC) $(CFLAGS) server.o sharedFunc.o -o server379

client.o: client.c sharedFunc.o sharedFunc.h
	$(CC) -c $(CFLAGS) client.c

server.o: server.c sharedFunc.o sharedFunc.h
	$(CC) -c $(CFLAGS) server.c

sharedFunc.o: sharedFunc.c
	$(CC) -c $(CFLAGS) sharedFunc.c
# mySignals.o: mySignals.c
# 	$(CC) -c $(CFLAGS) mySignals.c

clean:
	rm *o client379 server379 
	
