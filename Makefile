CC = gcc

# remove if not needed
CFLAGS =

DEPS =

all: client379 server379

client379: client.o
	$(CC) $(CFLAGS) client.o -o client379

server379: server.o
	$(CC) $(CFLAGS) server.o -o server379

client.o: client.c
	$(CC) -c $(CFLAGS) client.c

server.o: server.c
	$(CC) -c $(CFLAGS) server.c

clean:
	rm *o client379 server379 
	
