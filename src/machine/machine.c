#include "machine.h"

// retrieve all necessary port numbers, and the number of machines
void getArgs(int argc, char** argv, int* writeTo) {
  if (!isEnoughArgs(argc)) { exit(1); }

  // own, daemon port numbers respectively 
  int iArg1 = getPortNumber(argv[1]);
  int iArg2 = getPortNumber(argv[2]);

  // number of machines in the system
  int iArg3 = atoi(argv[3]);
  if (iArg3 != 0 && iArg3 != 1) {
    perror("Please enter a positive number of machines.\n");
    exit(1);
  }

  writeTo[0] = iArg1;
  writeTo[1] = iArg2;
  writeTo[2] = iArg3;

  return;
}

// parses a port number from argc
int getPortNumber(char* cArg) {
  int iArg = atoi(cArg);
  if (iArg1 <= 0 || iArg1 > MAX_PORT_NUMBER) {
    perror("Please enter a port number 1 <= num <= 65535.\n");
    exit(1); 
  }

  return iArg;
}

void* interaction(void* arg) {
  return NULL;
}

// verify the number of args passed to main
int isEnoughArgs(int argc) {
  if (argc == NUM_EXPECTED_ARGS) {
    return 1;
  }

  perror("Please enter these args: <portNumber> <daemonPortNumber> <numMachines>\n");
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

