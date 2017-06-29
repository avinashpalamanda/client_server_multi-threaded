// Copyright [2017] <Avinash Palamanda>
// The File implements the pingsites thread and queue implementation

#ifndef PING_
#define PING_

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
#include<limits.h>


// Structure which acts as a DB
struct ping_status{
  int thread_id;
  char Site_Name[128];
  float minimum;
  float maximum;
  float average;
  char status[128];
};

// Structure to store various handles
struct handlers{
  struct ping_status DB[100];
}handle[1000];
extern int handle_count;

// Structure to store queue
struct queue {
  int sockFD;
  char Site_Name[128];
  int handle_number;
  struct queue* next;
}*head,*tail;
pthread_mutex_t mutex;


void enqueue(int handle_number,char Site_Name[128],int sockFD);  // enqueue to queue
void update_database(int handle_number,float latency, int index, int count);  // Dequeue to queue
void GetTimestamp(struct timespec* temp);  // Timestamp of at that point
void* ping();  // Pingsite thread

#endif
