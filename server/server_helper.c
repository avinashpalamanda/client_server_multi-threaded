// Copyright [2017] <Avinash Palamanda>
// The File implements the helper function for the server

#include "server_helper.h"
#include "ping.h"

int handle_count=0;  //  To keep count of number of handles

// Function to print usage/help command
void usage() {
  system("clear");
  printf("            Usage        \n");
  printf("Handle Status      -  showHandleStatus [Handle Id]\n");
  printf("All Handles        -  showHandles\n");
  printf("Exit               -  exit\n");
  printf("Help               -  help\n");
  printf("ENTER THE COMMAND WORDS AND FOLLOW THE INSTRUCTION\n");
  fflush(stdout);
}


// Hash function to store the Site_Name
// Used string hashing technique
int hash_function(char Site_Name[128]) {
  long long hash=0;
  long long p=100;
  for(int i=0;Site_Name[i]!='\0';i--) {
    hash=(((hash*263+Site_Name[i])%p)+p)%p;
  }
  return hash;
}

// Send Handle details of particular handle id
// Input Socket FD
// Output Sends the handle status to client
void showHandles_handle(int sockFD) {
   int j;
   printf("Waiting for handle id\n");
   int nbytes = recv(sockFD,&j,sizeof(int),0);  // Recieve handle id
   if(nbytes<=0) { // If error in reception
     printf("Recieve failed\n");
   }

   char result[2000];
   for (int i=0;i<100;i++) {
     if (strcmp(handle[j].DB[i].Site_Name, "")) {  // Loop through check if it is filled
     sprintf(&result, "%s\n %d %s %f %f %f %s\n", result, j, handle[j].DB[i].Site_Name, handle[j].DB[i].average,
                                      handle[j].DB[i].minimum, handle[j].DB[i].maximum, handle[j].DB[i].status);
      printf("%s", result);  // Printing result
     }
   }
   send(sockFD,result,strlen(result),0);  // Sending the result to tyhe client
   result[0]='\0';
}

// Print site details of particular handle id
void showHandleStatus_server(){

   int j;
   scanf("%d",&j);  // Get handle id

   char result[2000];
   for (int i = 0; i<100; i++) {  // Loop through check if it is filled
     if (strcmp(handle[j].DB[i].Site_Name, "")){
     sprintf(&result, "%d %s %f %f %f %s\n", j, handle[j].DB[i].Site_Name, handle[j].DB[i].average, handle[j].DB[i].minimum,
                                                                            handle[j].DB[i].maximum, handle[j].DB[i].status);
     printf("%s", result);  // Printing result
     }
   }
   result[0]='\0';
}

// Print details of all handles
void showHandles_server() {

  char result[2000];
  for (int j=0; j<100; j++)  // All handles
    for (int i=0; i<100; i++)  // All Hash datas
      if(strcmp(handle[j].DB[i].Site_Name, "")) {
        sprintf(&result, "%d %s %f %f %f %s\n", j, handle[j].DB[i].Site_Name, handle[j].DB[i].average, handle[j].DB[i].minimum,
                                                                               handle[j].DB[i].maximum,handle[j].DB[i].status);
        printf("%s",result);  // Printing the result
      }
}

// Send details of all handles
void showHandles(int sockFD) {
  int handle_no=0;

  char result[2000];
  for (int j = 0; j < 100; j++)  // All handles
    for(int i = 0; i<100; i++)  // All Hash datas
      if(strcmp(handle[j].DB[i].Site_Name, "")){
        sprintf(&result, "%s\n %d %s %f %f %f %s\n", result, j, handle[j].DB[i].Site_Name, handle[j].DB[i].average,
                                          handle[j].DB[i].minimum, handle[j].DB[i].maximum, handle[j].DB[i].status);
      }
  send(sockFD,result,strlen(result),0);  // Sending the result
  result[0]='\0';
}

//Thread for individual client sockets
void* check(void* x_sockFD) {
  int sockFD = x_sockFD;

  while (1) {  // Loop continously to observer the socket
    const char s[2] = ",";
    char command[128];
    int nbytes=recv(sockFD, &command, 128*sizeof(char), 0);  // Recieve the command

    if (nbytes <= 0) {  // Reception error
      printf("recieving input failed - Command\n");
      return 0;
    }

    if (!(strcmp(command,"prin"))) {  // Send all handles details
      showHandles(sockFD);
    }

    else if (!(strcmp(command,"pria"))) {  // Send handle details of particular handle
      showHandles_handle(sockFD);
    }

    else if (!strcmp(command,"ping")) {  // Enqueue the pingsites
      char Site_Name[1000];
      nbytes = recv(sockFD, &Site_Name, 1000*sizeof(char), 0);
      if (nbytes <= 0) {  // Recpetion error
        printf("recieving input failed - Command\n");
        return 0;
      }

      char* token;
      token = strtok(Site_Name,s);
      while (token != NULL) {  // Loop thru tokens and enqueue them
        int hash = hash_function(token);
        strcpy(handle[handle_count].DB[hash].Site_Name,token);
        handle[handle_count].DB[hash].minimum = INT_MAX;
        handle[handle_count].DB[hash].maximum = INT_MIN;
        handle[handle_count].DB[hash].average = 0;
        strcpy(handle[handle_count].DB[hash].status, "INQUEUE");
        enqueue(handle_count, token, sockFD);
        token=strtok(NULL, s);
      }
      send(sockFD, &handle_count, 1*sizeof(int), 0);  // Send the handle id
      handle_count++;
      if(handle_count > 1000)  // Wrap thru handle count
        handle_count = 0;
    }
    pthread_mutex_unlock(&mutex);
  }
  return 0;
}


char* address(int sockFD){
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  getsockname (sockFD, (struct sockaddr *) &addr, &len);
  char* ip = (char*) malloc (128*sizeof(char));
  ip = inet_ntoa(addr.sin_addr);
  int port = ntohs(addr.sin_port);
  sprintf(ip, "%s %d", ip, port);
  return ip;
}
