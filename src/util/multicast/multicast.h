#ifndef __MULTICAST_H__
#define __MULTICAST_H__

#include <pthread.h>
#include "../socks/socks.h"

#define BUFF_SIZE 1024
#define MIN_PORT 10000

typedef struct {
  int port;
  int numMachines;
  int* vectorClock;
} args_causal_t;

typedef struct {
  int port;
  int numMachines;
} args_normal_t;

// need mutex I think

void* initInteractionCausal(void* args);
void* initInteractionNormal(void* args);
void joinCausalNetwork(int port, int numMachines, int* vectorClock);
void joinNormalNetwork(int port, int numMachines);
void* respInteractionCausal(void* args);
void* respInteractionNormal(void* args);

#endif
