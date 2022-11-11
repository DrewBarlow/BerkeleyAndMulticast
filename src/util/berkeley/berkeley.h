#ifndef __BERKELEY_H__
#define __BERKELEY_H__

#include <pthread.h>
#include "../socks/socks.h"

#define BUFF_SIZE 1024

extern pthread_mutex_t clockLock;
extern pthread_mutex_t threadLock;
extern int numClocksReceived;
extern int newClock;

void berkeleySync(int port, int daemonPort, int numMachines, int* logicalClock);
void daemonInit(int daemonPort, int numMachines, int* logicalClock);
void* daemonInteraction(void* arg);
int isDaemon(int port, int daemonPort);
void normalInit(int daemonPort, int* logicalClock);
int normalInteraction(int sockfd, int clifd, int* logicalClock);
void itoa(int n, char* s);

#endif
