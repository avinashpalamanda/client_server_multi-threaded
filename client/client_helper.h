// Copyright [2017] <Avinash Palamanda>
// The File implements the helper functions of client

#ifndef CLIENT_HELPER_
#define CLIENT_HELPER_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

int ping(int sockFD);
int print(char handle_number[],int sockFD);
int usage();

#endif
