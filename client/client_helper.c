// Copyright [2017] <Avinash Palamanda>
// The File implements the helper functions of client

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


// Function to send list of sites and display the handle id
// Input : Socket FD
int ping(int sockFD) {
  if (send(sockFD , "ping", 4 , 0 ) < 0) { // Notify server about pingsite command
    printf("Send failed");
    return 1;
  }

  char x[1000];
  printf("Enter the sites list : ");
  scanf("%s",x);

  if (send( sockFD , x, strlen(x) , 0 ) < 0) { // Send the site list
    printf("Send failed");
    return 1;
  }

  //Get handle ID's
  int handle;
  int nbytes=recv(sockFD, &handle, 1*sizeof(int), 0);
  if (nbytes <= 0) { // If there is error in recieving
    printf("recieving input failed - Command\n");
    return 1;
  }
  printf("Handle Number %d\n",handle);
  fflush(stdout);
  return 0;
}

// Display Usage
int usage(){
  printf("            Usage        \n");
  printf("Pinging Site       -  pingSites\n");
  printf("Handle Status      -  showHandleStatus\n");
  printf("All Handles        -  showHandles\n");
  printf("Exit               -  exit\n");
  printf("Help               -  help\n");
  printf("ENTER THE COMMAND WORDS AND FOLLOW THE INSTRUCTION\n");
  fflush(stdout);
  return 0;
}
