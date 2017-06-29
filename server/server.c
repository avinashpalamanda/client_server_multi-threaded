// Copyright [2017] <Avinash Palamanda>
// The File implements the main function for server.It creates socket and binds and run the accept fucntion. It also creates thread pool for the pingSites

#include "server_helper.h"
#include "server.h"
#include "ping.h"

int main(){

  system("clear");
  printf("Server Started\n");
  usage();
  head=NULL;  // Intialising the head and tail of linkedlist for Queue operation
  tail=NULL;

  struct sockaddr_in server, client;  // Structture for storing server and client address.
  socklen_t clilen;

  fd_set master;  // FD variables to keep track of new FD and monitor the FD already in register
  fd_set read_fds;
  FD_ZERO(&master);  // Setting up all intial FD register to 0
  FD_ZERO(&read_fds);

  int sockFD, newFD;  // Server Socket and Client Socket
  int yes = 1;

  // get us a socket and bind it to port
  sockFD = socket(AF_INET, SOCK_STREAM, 0);
  if (sockFD < 0) {
    perror("Error in Creating a Socket\n");
    exit(0);
  }

  // lose the pesky "address already in use" error message
  setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;  // Internet
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( 3000 );  // Port

  if (bind(sockFD, (struct sockaddr *)&server , sizeof(server)) < 0) { // Binding Sockets
    close(sockFD);
    perror("Error in Binding Socket\n");
    exit(0);
  }

  listen(sockFD, 100);  // Can connect upto 100 incoming sockets

  FD_SET(sockFD, &master);  // Adding the server socket to Master set
  FD_SET(0, &master);  // Adding the STDIN to Master set

  int fdmax = sockFD;  // keep track of the biggest file descriptor

  pthread_t tid[5];  // Creating threads dedicated for pingSites command

  for (int i = 0;i < 5; i++) { // Starting  threads dedicated to pingSites command
    pthread_create(&tid[i], NULL, ping,(void*) NULL);
  }

  while(1) {  // Run infinite loop on select call
    read_fds = master;  // copy it
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {  // Run select call and check for error
      perror("Error in select call\n");
      exit(0);
    }

    for (int i = 0;i <= fdmax; i++) {  // Check all FD
      if (FD_ISSET(i, &read_fds)) {  // Check if FD is set
        if (i == sockFD) {  // To check if there is need for accept call.
          clilen = sizeof(client);
          newFD = accept(sockFD, (struct sockaddr *)&client, &clilen);  // Accept a new client
          if (newFD < 0) {  // Check if the FD recived is error
            perror("ERROR on accept");
            exit(1);
          }
          else {  // Create a dedicated thread for accepted socket
            printf("Connection from %s accepted\n", inet_ntoa(client.sin_addr));  // Notify in command line about the new connection
            FD_SET(newFD, &master);  // Add the socket to FD set
            if(fdmax < newFD) {  // Update the fdmax
              fdmax = newFD;
            }
          }
          pthread_t tid;
          pthread_create(&tid, NULL, check,(void*) newFD);  // Creating thread for client
        }
        else if (i == 0) {  // Get the command line input STDIN
          char command[128];
          scanf("%s", command);

          if (strcmp(command, "help") == 0)  // Printing all handles in server
            usage();

          else if (strcmp(command, "showHandleStatus") == 0)  // Printing all handles in server
            showHandleStatus_server();

          else if (strcmp(command, "showHandles") == 0)  // Printing a particular handler
            showHandles_server();

          else if (strcmp(command, "exit") == 0) {  // Exit from server
            printf("Exiting from Server\n");
            close(sockFD);
            return 0;
          }
        }  //  End - Get the command line input STDIN
      }  //  End Check if FD is set
    }  //  End Check all FD
  }  //  While Loop end
}  // Main loop end
