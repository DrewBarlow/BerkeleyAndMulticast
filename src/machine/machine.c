#include "machine.h"

// deallocate initialized vector clock
void destroyVectorClock(int* vectorClock) {
  free(vectorClock);
  return;
}

// retrieve all necessary port numbers, and the number of machines
void getArgs(int argc, char** argv, int* writeTo) {
  if (!isEnoughArgs(argc)) { exit(1); }

  // own, daemon port numbers respectively 
  int iArg1 = getPortNumber(argv[1]);
  int iArg2 = getPortNumber(argv[2]);

  // number of machines in the system
  int iArg3 = atoi(argv[3]);
  if (iArg3 < 3) {
    perror("Please choose for there to be at least 3 machines.\n");
    exit(1);
  }

  // multicast is causal ({0,1})
  int iArg4 = atoi(argv[4]);
  if (iArg4 != 0 && iArg4 != 1) {
    perror("Please denote whether or not messaging is causal.\n");
    exit(1);
  }

  writeTo[0] = iArg1;
  writeTo[1] = iArg2;
  writeTo[2] = iArg3;
  writeTo[3] = iArg4;

  return;
}

// parses a port number from argc
int getPortNumber(char* cArg) {
  int iArg = atoi(cArg);
  if (iArg <= 0 || iArg > MAX_PORT_NUMBER) {
    perror("Please enter a port number 1 <= num <= 65535.\n");
    exit(1); 
  }

  return iArg;
}

// initialize multicast messages and buffers
void initMulticastMessages(char messages[NUM_MULTICAST_MESSAGES][BUFF_SIZE]) {
  for (int i = 0; i < NUM_MULTICAST_MESSAGES; i++) {
    bzero(messages[i], BUFF_SIZE);
  }

  strncpy(messages[0], "Multicast Message #1: Hello!", 28);
  strncpy(messages[1], "Multicast Message #2: Yoyoyoyo", 30);
  strncpy(messages[2], "Multicast Message #3: Okay we're done", 37);

  return;
}

// initialize vector clocks (ALLOCATES MEMORY THAT NEEDS TO BE FREED)
void initVectorClock(int** vectorClock, int myId, int logicalClock, int numMachines) {
  *vectorClock = calloc(numMachines, sizeof(int));
  bzero(*vectorClock, numMachines);
  (*vectorClock)[myId] = logicalClock;

  return;
}

void* interaction(void* arg) {
  return NULL;
}

// verify the number of args passed to main
int isEnoughArgs(int argc) {
  if (argc == NUM_EXPECTED_ARGS) {
    return 1;
  }

  perror("Please enter these args: <portNumber> <daemonPortNumber> <numMachines> <isCausal>\n");
  return 0;
}

// simple buffer print
void printBuff(char *buff, int size) {
  for (int i = 0; i < size; i++) {
    printf("%c", buff[i]);
  }

  printf("\n");
  return;
}

