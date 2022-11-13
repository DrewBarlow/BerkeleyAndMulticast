#ifndef __MULTICAST_H__
#define __MULTICAST_H__

#include <pthread.h>
#include "../socks/socks.h"

#define BUFF_SIZE 1024
#define MIN_PORT 10000

typedef struct argStruct {
  char* arg1; 
  int arg2;
} args_t;

// need mutex I think

void* initInteractionCausal(void* args);
void* initInteractionNormal(void* args);
void initMachine(int port);
void multicast(char* msg, int port, int numMachines, int* vectorClock, int isCausal);
void* respInteractionCausal(void* args);
void* respInteractionNormal(void* args);

#endif
