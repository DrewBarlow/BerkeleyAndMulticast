#include "machine.h"

int logicalClock;

// arg structure:
// 0     1        2        3
// fname respPort initPort numMachines 
int main(int argc, char** argv) {
  // parse cmd line args
  int parsed[3];
  getArgs(argc, argv, parsed);
  int respPort = parsed[0];
  int initPort = parsed[1];
  int numMachines = parsed[2];

  // randomly initialize logical clock
  srand(time(NULL));
  logicalClock = rand() % CLOCK_RANDOM_CEILING;

  // use Berkeley algorithm to synchronize and take avg of clocks
  // how should I choose the time daemon? idfk lol
  berkeleySync(respPort, initPort);
  printf("CLOCK AFTER SYNC: %d\n", logicalClock);
  
  // spawn two threads:
  // 1. sending multicast to other nodes using initPort
  // 2. listening for multicast on respPort
  // USE VECTOR CLOCKS

  return 0;
}

