#include "machine.h"

int* vectorClock;

// arg structure:
// 0     1    2          3           4
// fname port daemonPort numMachines isCausal 
int main(int argc, char** argv) {
  // parse cmd line args
  int parsed[4];
  getArgs(argc, argv, parsed);
  int port = parsed[0];
  int daemonPort = parsed[1];
  int numMachines = parsed[2];
  int isCausal = parsed[3];

  // randomly initialize logical clock
  srand(time(NULL));
  int logicalClock = rand() % CLOCK_RANDOM_CEILING;

  // use Berkeley algorithm to synchronize and take avg of clocks
  // time daemon selected upon program execution
  int myId = port - daemonPort;
  if (myId == 0) {
    printf("Randomly initialized clock value (Daemon): %d\n", logicalClock);
  } else {
    printf("Randomly initialized clock value (Machine #%d): %d\n", myId, logicalClock);
  }

  berkeleySync(port, daemonPort, numMachines, &logicalClock);

  if (myId == 0) {
    printf("Synchronization process finished. New clock value (Daemon): %d\n", logicalClock);
  } else {
    printf("Synchronization process finished. New clock value (Machine #%d): %d\n", myId, logicalClock);
  }

  if (isCausal) {
    initVectorClock(&vectorClock, myId, logicalClock, numMachines);
    joinCausalNetwork(port, numMachines, vectorClock);
    destroyVectorClock(vectorClock);
  } else {
    joinNormalNetwork(port, numMachines);
  }

  // init multicast messages and vector clock, then send them
  // char messages[NUM_MULTICAST_MESSAGES][BUFF_SIZE];
  // initMulticastMessages(messages);

  // for (int i = 0; i < NUM_MULTICAST_MESSAGES; i++) {
  //   multicast(messages[i], port, numMachines, vectorClock, isCausal);
  // }
  

  return 0;
}

