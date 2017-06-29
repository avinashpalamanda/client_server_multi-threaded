// Copyright [2017] <Avinash Palamanda>
// The File implements the helper function for the server

#ifndef SERVER_HELPER_
#define SERVER_HELPER_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

void* check(void* x_sockFD);  // specfied thread for each client
char* address(int sockFD);  // Returns ip and port no.
void showHandles(int sockFD);  // Send all handle details
void showHandles_server();  // Print all handles
void showHandles_handle(int sockFD);  // Send Details of particular handle
void showHandleStatus_server();  // Print particular handle
int hash_function(char Site_Name[128]);  // Calculates hash on string
void usage();  // Displays usage
#endif
