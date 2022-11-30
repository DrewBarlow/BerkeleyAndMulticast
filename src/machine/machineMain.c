#include "machine.h"

int* vectorClock;

// arg structure:
// 0     1    2          3      
// fname port daemonPort numMachines  
int main(int argc, char** argv) {
  // parse cmd line args
  int parsed[3];
  getArgs(argc, argv, parsed);
  int port = parsed[0];
  int daemonPort = parsed[1];
  int numMachines = parsed[2];

  // randomly initialize logical clock
  srand(time(NULL));
  int logicalClockOrig = rand() % CLOCK_RANDOM_CEILING;
  int logicalClock = logicalClockOrig;

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
    printf("\n~~~ MULTICAST STUFF ~~~\n");
  } else {
    printf("Synchronization process finished. New clock value (Machine #%d): %d\n", myId, logicalClock);
  }

  sleep(1);

  // the amount of time to wait before this machine
  // sends its multicast
  int delay = logicalClockOrig % 3;
  
  // initialize the vector clock for this machine, then run 
  // a multicast protocol
  initVectorClock(&vectorClock, myId, numMachines);
  joinNetwork(port, numMachines, delay, vectorClock);
  destroyVectorClock(vectorClock);

  return 0;
}

