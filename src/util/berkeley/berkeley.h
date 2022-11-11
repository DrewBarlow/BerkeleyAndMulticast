#ifndef __BERKELEY_H__
#define __BERKELEY_H__

#include "../socks/socks.h"

void berkeleySync(int port, int daemonPort, int numMachines, int* logicalClock);
void daemonInit(int daemonPort, int numMachines);
int isDaemon(int port, int daemonPort);
void normalInit(int daemonPort, int* logicalClock);

#endif
