#ifndef __CAUSAL_H__
#define __CAUSAL_H__

#include <pthread.h>
#include "../socks/socks.h"

#define BUFF_SIZE 1024
#define MIN_PORT 10000

typedef struct {
  int port;
  int numMachines;
} args_cast_t;

void* initInteraction(void* args);
void joinNetwork(int port, int numMachines);
void* respInteraction(void* args);

#endif
