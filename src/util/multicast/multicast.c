#include "multicast.h"

void* initInitCausal(void* args) {
  args_causal_t args = *((args_causal_t*) args);
  int sockfd;
  struct sockaddr_in serverAddr;

  // create the listening socket
  if (makeRespSock(args.port, args.numMachines - 1, &sockfd, &serverAddr) != -1) {
    int currThread = 0;

    // while loop here?
    int connfd = acceptClient(sockfd, &serverAddr);

    if (connfd != -1) {
      // spawn thread?
    }
  }
  
  return NULL;
}

void* initInitNormal(void* args) {
  args_normal_t args = *((args_normal_t*) args);

  return NULL;
}

void* initInteractionCausal(void* args) {
  args_causal_t args = *((args_causal_t*) args);
  
  return NULL;
}

void* initInteractionNormal(void* args) {
  args_normal_t args = *((args_normal_t*) args);

  return NULL;
}

void joinCausalNetwork(int port, int numMachines, int* vectorClock) {
  pthread_t initThread;
  pthread_t respThread;

  // init the args in a struct, because void*
  args_causal_t args;
  args.port = port;
  args.numMachines = numMachines;
  args.vectorClock = vectorClock;

  // spawn the listening and communicating threads
  int threadRet = pthread_create(&initThread, NULL, initInitCausal, (void*) &args);
  if (threadRet != 0) {
    perror("Failed to spawn causal init thread.\n");
    exit(1);
  }

  threadRet = pthread_create(&respThread, NULL, respInitCausal, (void*) &args);
  if (threadRet != 0) {
    perror("Failed to spawn causal resp thread.\n");
    exit(1);
  }

  return;
}

void joinNormalNetwork(int port, int numMachines) {
  pthread_t initThread;
  pthread_t respThread;

  // init the args in a struct, because void*
  args_normal_t args;
  args.port = port;
  args.numMachines = numMachines;

  // spawn the listening and communicating threads
  int threadRet = pthread_create(&initThread, NULL, initInitNormal, (void*) &args);
  if (threadRet != 0) {
    perror("Failed to spawn normal init thread.\n");
    exit(1);
  }

  threadRet = pthread_create(&respThread, NULL, respInitNormal, (void*) &args);
  if (threadRet != 0) {
    perror("Failed to spawn normal resp thread.\n");
    exit(1);
  }

  return;
}

void* respInitCausal(void* args) {
  args_causal_t args = *((args_causal_t*) args);

  return NULL;
}

void* respInitNormal(void* args) {
  args_normal_t args = *((args_normal_t*) args);
  
  return NULL;
}

void* respInteractionCausal(void* args) {
  args_causal_t args = *((args_causal_t*) args);

  return NULL;
}

void* respInteractionNormal(void* args) {
  args_normal_t args = *((args_normal_t*) args);
  
  return NULL;
}

