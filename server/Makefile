#Makefile for the DNS_Performance_Tracker#
CC=gcc

CFLAGS= -w
CFLAGS1= -c
LDFLAGS= -lpthread

all:	server

#Linking all the object file
server:	server.o server_helper.o ping.o
	$(CC) -o server server.o server_helper.o ping.o  $(CFLAGS) $(LDFLAGS)

#main function object file
server.o:	server.c
	$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -o $@ $^

#Controller Query object file
server_helper.o:	server_helper.c
	$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -o $@ $^

#DB model object file
ping.o:	ping.c
	$(CC) $(CFLAGS1) $(CFLAGS) $(LDFLAGS) -o $@ $^

#DB Removing all files
clean:
	rm -rf *o server
