#include "multicast.h"

pthread_mutex_t vClockLock;

void* initInit(void* fargs) {
  args_cast_t args = *((args_cast_t*) fargs);

  // used for keeping track of socket fds and threads
  pthread_t threads[args.numMachines];
  int sockfds[args.numMachines];
  int clifds[args.numMachines];
  bzero(threads, args.numMachines);
  bzero(sockfds, args.numMachines);
  bzero(clifds, args.numMachines);

  // used for calls to socket creation fxns
  int ret = 0;
  int writeTo[2];

  // make outgoing sockets for each machine on the network
  for (int i = 0; i < args.numMachines; i++) {
    if (i == args.port - MIN_PORT) {
      sockfds[i] = -1;
      clifds[i] = -1;
    } else {
      ret = makeInitSock(args.port, writeTo);
      if (ret != 0) {
        perror("Error occurred when making initiator socket.\n");
        exit(1);
      }

      sockfds[i] = writeTo[0];
      clifds[i] = writeTo[1];
    }
  }

  // spawn threads for each socket 
  for (int i = 0; i < args.numMachines; i++) {
    if (sockfds[i] != -1) {
      args_cast_t threadArgs;
      threadArgs.port = MIN_PORT + i;
      threadArgs.numMachines = args.numMachines;
      threadArgs.sockfd = sockfds[i];
      threadArgs.vectorClock = args.vectorClock;

      ret = pthread_create(&threads[i], NULL, initInteraction, (void*) &threadArgs);
      if (ret != 0) {
        perror("Error occurred when spawning initiator threads.\n");
        exit(1);
      }
    }
  }

  for (int i = 0; i < args.numMachines - 1; i++) {
    pthread_join(threads[i], NULL);
  }

  return NULL;
}

// sends over this machine's vectorClock and a message
void* initInteraction(void* fargs) {
  args_cast_t args = *((args_cast_t*) fargs);
  char sendBuff[BUFF_SIZE];
  bzero(sendBuff, BUFF_SIZE);

  // prepare multicast message
  char id[BUFF_SIZE];
  bzero(id, BUFF_SIZE);
  itoa(args.port - MIN_PORT, id);
  strncpy(sendBuff, "Msg from machine ", 17);
  strncat(sendBuff, id, 5);

  // acquire the clockLock mutex so the vectorClock
  // does not update in the middle of being sent
  pthread_mutex_lock(&vClockLock);
  args.vectorClock[args.port - MIN_PORT]++;
  sendVectorClock(args.sockfd, args.numMachines, args.vectorClock);
  int ret = write(args.sockfd, sendBuff, BUFF_SIZE);
  pthread_mutex_unlock(&vClockLock);
    
  if (ret == -1) {
    perror("Failed to multicast message.\n");
    exit(1);
  }

  return NULL;
}

void joinNetwork(int port, int numMachines, int* vectorClock) {
  // initialize the mutex for this machine's vector clock
  pthread_mutex_init(&vClockLock, NULL);
  pthread_t initThread;
  pthread_t respThread;

  // init the args in a struct, because void*
  args_cast_t args;
  args.port = port;
  args.numMachines = numMachines;
  args.sockfd = -1;
  args.vectorClock = vectorClock;

  // spawn the listening and communicating threads
  int threadRet = pthread_create(&initThread, NULL, initInit, (void*) &args);
  if (threadRet != 0) {
    perror("Failed to spawn normal init thread.\n");
    exit(1);
  }

  threadRet = pthread_create(&respThread, NULL, respInit, (void*) &args);
  if (threadRet != 0) {
    perror("Failed to spawn normal resp thread.\n");
    exit(1);
  }

  // join the threads to end communication
  pthread_join(initThread, NULL);
  pthread_join(respThread, NULL);
  pthread_mutex_destroy(&vClockLock);

  return;
}

// allocates memory that needs to be freed
int* recvVectorClock(int sockfd, int numMachines) {
  // allocate enough memory for the vector clock we will receive
  int* newClock = calloc(numMachines, sizeof(int));
  char recvBuff[BUFF_SIZE];
  bzero(recvBuff, BUFF_SIZE);
  int ret = 0;

  // receive a message for each slot in the clock, convert to int
  for (int i = 0; i < numMachines; i++) {
    ret = read(sockfd, recvBuff, BUFF_SIZE);
    if (ret == -1) {
      perror("Failed to read vector clock from buffer.\n");
      exit(1);
    }

    newClock[i] = atoi(recvBuff);
    bzero(recvBuff, BUFF_SIZE);
  }

  return newClock;
}

void* respInit(void* fargs) {
  args_cast_t args = *((args_cast_t*) fargs);
  int sockfd;
  struct sockaddr_in serverAddr;
  pthread_t threads[args.numMachines - 1];

  // create the listening socket
  if (makeRespSock(args.port, args.numMachines - 1, &sockfd, &serverAddr) != -1) {
    int currThread = 0;

    // spin numMachines - 1 threads (arbitrary for consistency)
    while (currThread != args.numMachines - 1) {
      int connfd = acceptClient(sockfd, &serverAddr);

      if (connfd != -1) {
        args_cast_t threadArgs;
        threadArgs.port = args.port;
        threadArgs.numMachines = args.numMachines;
        threadArgs.sockfd = connfd;
        threadArgs.vectorClock = args.vectorClock;

        // spawn a thread to receive the vector clock and message
        int threadRet = pthread_create(&threads[currThread++], NULL, respInteraction, (void*) &threadArgs);
        if (threadRet != 0) {
          perror("Error spawning a responder thread.\n");
          exit(1);
        }
      } else {
        perror("Error accepting a client.\n");
        exit(1);
      }
    }

    // join the threads and finish execution
    for (int i = 0; i < args.numMachines - 1; i++) {
      pthread_join(threads[i], NULL);
    }

  } else {
    perror("Error creating listening socket.\n");
    exit(1);
  }

  return NULL;
}

// receives a machines vector clock and a message from it
void* respInteraction(void* fargs) {
  args_cast_t args = *((args_cast_t*) fargs);
  char recvBuff[BUFF_SIZE];
  bzero(recvBuff, BUFF_SIZE);

  // receive the machine's vector clock 
  int* otherClock = recvVectorClock(args.sockfd, args.numMachines);
  pthread_mutex_lock(&vClockLock);

  // update this machine's vector clock
  args.vectorClock[args.port - MIN_PORT]++;
  updateVectorClock(args.vectorClock, otherClock, args.numMachines);

  // receive the message
  int ret = read(args.sockfd, recvBuff, BUFF_SIZE);
  if (ret == -1) {
    perror("Failed to read multicast from buffer.\n");
    pthread_mutex_unlock(&vClockLock);
    exit(1);
  }

  printf("Machine %d received a message:\n\t%s\n", (args.port - MIN_PORT), recvBuff);
  printf("Received a message");

  pthread_mutex_unlock(&vClockLock);
  free(otherClock);
  return NULL;
}

void sendVectorClock(int connfd, int numMachines, int* vectorClock) {
  char sendBuff[BUFF_SIZE];
  bzero(sendBuff, BUFF_SIZE);
  int ret = 0;

  // send each element of this machine's vector clock to
  // the listening machine
  for (int i = 0; i < numMachines; i++) {
    itoa(vectorClock[i], sendBuff);
    ret = write(connfd, sendBuff, BUFF_SIZE);

    if (ret == -1) {
      perror("Failed to write vector clock to buffer.\n");
      exit(1);
    }

    bzero(sendBuff, BUFF_SIZE);
  }

  return;
}

void updateVectorClock(int* thisClock, int* thatClock, int numMachines) {
  int max(int a, int b) { return (a > b) ? a : b; }

  for (int i = 0; i < numMachines; i++) {
    thisClock[i] = max(thisClock[i], thatClock[i]);
  }

  return;
}
