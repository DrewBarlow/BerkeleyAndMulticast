#ifndef __BERKELEY_H__
#define __BERKELEY_H__

#include <pthread.h>
#include "../socks/socks.h"

extern pthread_mutex_t clockLock;
extern pthread_mutex_t threadLock;
extern int numClocksReceived;
extern int newClock;

void berkeleySync(int port, int daemonPort, int numMachines, int* logicalClock);
void daemonInit(int daemonPort, int numMachines, int* logicalClock);
int isDaemon(int port, int daemonPort);
void normalInit(int daemonPort, int* logicalClock);

#endif
