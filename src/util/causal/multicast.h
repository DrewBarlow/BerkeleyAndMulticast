#ifndef __MULTICAST_H__
#define __MULTICAST_H__

#include <pthread.h>
#include "../socks/socks.h"

#define BUFF_SIZE 1024
#define MIN_PORT 10000

typedef struct {
  int srcId;
  int port;
  int numMachines;
  int sockfd;
  int* vectorClock;
} args_cast_t;

extern pthread_mutex_t vClockLock;

int causalityReport(int* thisClock, int* thatClock, char* buff, int srcId, int numMachines);
void* initInit(void *fargs);
void* initInteraction(void* fargs);
void joinNetwork(int port, int numMachines, int delay, int* vectorClock);
void printVectorClock(int srcId, int* vectorClock, int numMachines);
int* recvVectorClock(int sockfd, int numMachines);  // allocates memory
void* respInit(void *fargs);
void* respInteraction(void* fargs);
void sendVectorClock(int connfd, int numMachines, int* vectorClock);
void updateVectorClock(int* thisClock, int* thatClock, int srcId, int numMachines);

#endif
