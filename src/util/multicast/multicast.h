#ifndef __MULTICAST_H__
#define __MULTICAST_H__

#include <pthread.h>
#include "../socks/socks.h"

#define BUFF_SIZE 1024
#define MIN_PORT 10000

typedef struct {
  int port;
  int numMachines;
  int sockfd;
  int* vectorClock;
} args_cast_t;

extern pthread_mutex_t clockLock;

void* initInteraction(void* args);
void itoa(int n, char* s);
void joinNetwork(int port, int numMachines, int* vectorClock);
int* recvVectorClock(int sockfd, int numMachines);  // allocates memory
void* respInteraction(void* args);
void sendVectorClock(int connfd, int numMachines, int* vectorClock);
void updateVectorClock(int* thisClock, int* thatClock, int numMachines);

#endif
