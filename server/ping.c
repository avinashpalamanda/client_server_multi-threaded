// Copyright [2017] <Avinash Palamanda>
// The File implements the pingsites thread and queue implementation

#include "ping.h"
#include "server_helper.h"

// Queue Functions
// Enqueue sitenames
// Input : Handle Number(to track the particular handle),Site List,socket FD
void enqueue(int handle_number,char Site_Name[128],int sockFD) {
  struct queue *current = malloc (sizeof(struct queue));

  strcpy(current->Site_Name,Site_Name);
  current->sockFD=sockFD;
  current->handle_number=handle_number;
  current->next=NULL;

  if(head==NULL) {  // First entry
    tail=current;
    head=current;
    return;
  }

  tail->next=current;  // Enqueue the data
  tail=current;
}

// Dequeue the queue
// Output : dequeued value
struct queue* dequeue() {
  pthread_mutex_lock(&mutex);
  if(head==NULL){
    return NULL;
  }
  struct queue* current=head;
  head=head->next;
  pthread_mutex_unlock(&mutex);
  return current;
}

// Updates the DB for partiular handle id and particular site.
// Input : Handle Number(to track the particular handle),new value to be updated,index,count of previous entries.
void update_database(int handle_number, float latency, int index, int count){
  if (handle[handle_number].DB[index].minimum > latency)  // Check the minimum value and update
    handle[handle_number].DB[index].minimum = latency;
  if (handle[handle_number].DB[index].maximum < latency)  // Check the maximum value and update
    handle[handle_number].DB[index].maximum = latency;
  handle[handle_number].DB[index].average = (handle[handle_number].DB[index].average*(count-1))+latency;
  handle[handle_number].DB[index].average = handle[handle_number].DB[index].average/count;
}

// Function is used to obtain the current system timestamp.
void GetTimestamp(struct timespec* temp) {
  if (timespec_get(temp, CLOCK_MONOTONIC) == -1) {  // In case if the timestamp is not able to be obtained, the code exits.
    printf("Error in Getting clock\n");
    exit(EXIT_FAILURE);
  }
}


//Ping threads dedicated only for dequeueing entry from queue and perform pinging operation
void* ping() {
  int hash;
  int handle_number;
  while(1) {  // Loop through continously tilll there is entry in queue
    struct queue *current = malloc (sizeof(struct queue));  // Dequeued item
    current = dequeue();  // Dequeue the queue
    if (current != NULL) {
      struct timespec start, stop;
      int sockfd1;
      struct addrinfo hints1, *servinfo1, *p1;
      int rv1;
      memset(&hints1, 0, sizeof hints1);
      hints1.ai_family = AF_UNSPEC;
      hints1.ai_socktype = SOCK_STREAM;
      hash = hash_function(current->Site_Name);
      handle_number = current->handle_number;
      //Trying to connect to a webserver from local.In this case connecting to google.com.
      //Creating socket and getting address info of google.com is taken from beej guide page 32 and 33.
      strcpy(handle[handle_number].DB[hash].status,"INPROCCESS");  // Updating status of the site

      for (int i = 0; i < 10; i++) {  // Send 10 ping requests
        if ((rv1 = getaddrinfo("google.com", "80", &hints1, &servinfo1)) != 0) {
      	   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv1));
        }

        // loop through all the results and make a socket
        for(p1 = servinfo1; p1 != NULL; p1 = p1->ai_next) {
      	   if ((sockfd1 = socket(p1->ai_family, p1->ai_socktype,p1->ai_protocol)) == -1) {
      	      perror("talker: socket");
      	       continue;
           }
           break;
        }

        GetTimestamp(&start);
      	if ((connect(sockfd1, p1->ai_addr, p1->ai_addrlen)) < 0) {
          close(sockfd1);
          perror("client: connect");
        }
        GetTimestamp(&stop);

        double latency = ((stop.tv_sec-start.tv_sec)*1000+(stop.tv_nsec-start.tv_nsec)/1000000);
        update_database(handle_number,latency,hash,i+1);
      }
    }
    char result[128];
    sprintf(&result,"%s %f %f %f",handle[handle_number].DB[hash].Site_Name, handle[handle_number].DB[hash].average,
                                    handle[handle_number].DB[hash].minimum, handle[handle_number].DB[hash].maximum);
    strcpy(handle[handle_number].DB[hash].status,"COMPLETED");
  }
}
