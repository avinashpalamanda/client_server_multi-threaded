// Copyright [2017] <Avinash Palamanda>
// The File is the main client function. It creates socket and connect to the server

#include "client_helper.h"
#include "client.h"

int main(int argc, char *argv[]) {
  system("clear");

  if (argc!=2) {  // Validate input
    printf("Please inter the ip address Example : ./client 127.0.0.1");
    fflush(stdout);
    return 0;
  }

  struct sockaddr_in server;  // Struct to store address of server
  fd_set master;  // FD variables to keep track of new FD and monitor the FD already in register
  fd_set read_fds;
  FD_ZERO(&master);  // Setting up all intial FD register to 0
  FD_ZERO(&read_fds);
  int sockFD;  // Server Socket

  // get us a socket and bind it
  sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0) {  // Error in creation of socket
      perror("Error in Creating a Socket\n");
      exit(0);
    }

   server.sin_addr.s_addr = inet_addr(argv[1]);
   server.sin_family = AF_INET;  // IP
   server.sin_port = htons( 3000 );  // Port Number

   //Connect to remote server
   if (connect(sockFD, (struct sockaddr *)&server , sizeof(server)) < 0) {  // Connect to server
     perror("connect failed. Error");
     exit(0);
   }

   FD_SET(0, &master);  // Insert and server socket STDIN into FD set
   FD_SET(sockFD, &master);
   int fdmax = sockFD;

   usage();  // Dis[lay help

   while (1) {  //Infinite while loop to select function
     char str[80];int j=0;  // Command variable

     printf("Enter command : ");
     fflush(stdout);
     scanf("%s",str);

     if (strcmp(str, "help") == 0) {  // Request for help
       system("clear");
       usage();
     }

     else if (strcmp(str, "pingSites") == 0) { // To Ping the website
       if (ping(sockFD))
          perror("Error in Pinging Sites\n");
     }

     else if (strcmp(str, "showHandles") == 0) { // To Print all handles
      send(sockFD,"prin",4*sizeof(char),0);

      char result[2000];
      int nbytes = recv(sockFD, &result, 2000, 0);  // Recieve result
      if (nbytes <= 0) { // Error recieving handle details
        printf("recieving input failed - Command\n");
        fflush(stdout);
        return 0;
      }
      printf("%s\n", result);
      fflush(stdout);
     }

     else if (strcmp(str, "showHandleStatus") == 0) { // To Print Handle of particukar id
      send(sockFD,"pria",4*sizeof(char),0);

      int x;
      printf("Enter Handle ID : ");
      fflush(stdout);
      scanf("%d",&x);
      send(sockFD,&x,1*sizeof(int),0);

      char result[2000];
      int nbytes=recv(sockFD, &result, 2000, 0);
      if (nbytes <= 0) { // If there is error in reciveing details of particular handle
        printf("recieving input failed - Command\n");
        fflush(stdout);
        return 0;
      }
      printf("%s\n",result);
      fflush(stdout);
     }

     else if(strcmp(str,"exit")==0) { // Exit client
       close(sockFD);
       return 0;
     }
     str[0]='\0';
   }
}
