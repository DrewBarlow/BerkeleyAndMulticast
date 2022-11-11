#include "machine.h"

//int logicalClock;

// arg structure:
// 0     1    2          3           4
// fname port daemonPort numMachines randomSeed
int main(int argc, char** argv) {
  // parse cmd line args
  int parsed[3];
  getArgs(argc, argv, parsed);
  int port = parsed[0];
  int daemonPort = parsed[1];
  int numMachines = parsed[2];

  // randomly initialize logical clock
  srand(time(NULL));
  int logicalClock = rand() % CLOCK_RANDOM_CEILING;

  // use Berkeley algorithm to synchronize and take avg of clocks
  // time daemon selected upon program execution
  printf("Randomly initialized clock value (Process #%d): %d\n", port - daemonPort, logicalClock);
  berkeleySync(port, daemonPort, numMachines, &logicalClock);
  printf("Synchronization process finished. New clock value (Process #%d): %d\n", port - daemonPort, logicalClock);
  
  // spawn two threads:
  // 1. sending multicast to other nodes using initPort
  // 2. listening for multicast on respPort
  // USE VECTOR CLOCKS

  return 0;
}

