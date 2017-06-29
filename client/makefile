#Makefile for the DNS_Performance_Tracker#
CC=gcc

CFLAGS= -w
CFLAGS1= -c
LDFLAGS= -lpthread

all:	client

#Linking all the object file
client:	client.o client_helper.o
	$(CC) -o client client.o client_helper.o $(CFLAGS) $(LDFLAGS)


#main function object file
client.o:	client.c
	$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -o $@ $^

#Controller Query object file
client_helper.o:	client_helper.c
	$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -o $@ $^

#DB Removing all files
clean:
	rm -rf *o client
