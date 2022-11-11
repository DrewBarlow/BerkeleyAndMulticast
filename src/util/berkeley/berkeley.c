#include "berkeley.h"

void berkeleySync(int port, int daemonPort, int numMachines, int* logicalClock) {
  if (isDaemon(port, daemonPort)) {
    daemonInit(daemonPort, numMachines, logicalClock);
    return;
  }

  normalInit(daemonPort, logicalClock);
  return;
}

// if the machine is the time daemon, let all other machines connect
// then interact with them accordingly
void daemonInit(int daemonPort, int numMachines, int* logicalClock) {
  // wait until (numMachines - 1) connect to the daemon,
  // then ask them all for their time and average it

  return;
}

// if the machine is the daemon:
// RECV "HELLO"
// SEND "ACK"
// RECV {ALL-CLOCK-VALUES}
// SEND {NEW-CLOCK-VALUE}
// RECV "BYE"
void daemonInteraction() {

  return;
}

// if this processes' port number is the same as the daemon's,
// it is the daemon
int isDaemon(int port, int daemonPort) {
  return port == daemonPort;
}

// if the machine is not the time daemon, connect to it
void normalInit(int daemonPort, int* logicalClock) {
  int writeTo[2];
  int ret = makeInitSock(daemonPort, writeTo);

  if (ret == 0) {
    int sockfd = writeTo[0];
    int clifd = writeTo[1]:
    
    normalInteraction(sockfd, clifd, logicalClock);
    close(clifd);
  }

  return;
}

// if the machine is not the daemon:
// SEND "HELLO"
// RECV "ACK"
// SEND <logicalClock>
// RECV <newClock>
// SEND "BYE"
void normalInteraction(int sockfd, int clifd, int* logicalClock) {

  return;
}

