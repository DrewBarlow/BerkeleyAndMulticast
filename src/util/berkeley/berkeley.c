#include "berkeley.h"

pthread_mutex_t clockLock;
pthread_mutex_t threadLock;
int numClocksReceived;
int newClock;

void berkeleySync(int port, int daemonPort, int numMachines, int* logicalClock) {
  if (isDaemon(port, daemonPort)) {
    newClock = *logicalClock;
    numClocksReceived = 0;
    pthread_mutex_init(&clockLock, NULL);
    pthread_mutex_init(&threadLock, NULL);

    daemonInit(daemonPort, numMachines, logicalClock);

    pthread_mutex_destroy(&clockLock);
    pthread_mutex_destroy(&threadLock);
    return;
  }

  normalInit(daemonPort, logicalClock);
  return;
}

// if the machine is the time daemon, let all other machines connect
// then interact with them accordingly
void daemonInit(int daemonPort, int numMachines, int* logicalClock) {
  // if a process fails to connect then just modify the number of active machines
  pthread_t threads[numMachines - 1];
  int sockfd;
  struct sockaddr_in serverAddr;
  
  // create the listening socket
  if (makeRespSock(daemonPort, numMachines - 1, &sockfd, &serverAddr) != -1) {
    int currThread = 0;

    pthread_mutex_lock(&threadLock);
    while (currThread != numMachines - 1) {
      // receive a connection from a non-daemon machine
      int connfd = acceptClient(sockfd, &serverAddr);

      if (connfd != -1) {
        // spawn a new thread to handle the machine
        int threadRet = pthread_create(&threads[currThread++], NULL, daemonInteraction, (void*) &connfd);

        if (threadRet != 0) {
          printf("Error spawning a thread for non-daemon machine.\n");
          break;
        }
      }
    }

    // AHHHHHHHHHHHHHHHHHHHH
    // wait until all threads have updated newClock
    while (numClocksReceived != numMachines - 1) {}
    newClock /= numMachines;

    *logicalClock = newClock;

    // update clock average and resume halted threads
    pthread_mutex_unlock(&threadLock);

    // join all threads
    for (int i = 0; i < numMachines - 1; i++) {
      pthread_join(threads[i], NULL);
    }

    printf("Daemon finished synchronization.\n");
  }

  return;
}

// if the machine is the daemon:
// RECV "HELLO"
// SEND "ACK"
// RECV {ALL-CLOCK-VALUES}
// SEND {NEW-CLOCK-VALUE}
// RECV "BYE"
void* daemonInteraction(void* arg) {
  int connfd = *((int*) arg);
  char buff[BUFF_SIZE];
  bzero(buff, BUFF_SIZE);

  // read from the socket
  int totalBytesRead = 0;
  int ret = read(connfd, buff, BUFF_SIZE);
  totalBytesRead += ret;
  if (ret == -1) {
    perror("Failed to read from buffer.\n");
    exit(1);
  }

  // read buffers until we have 1KB of data
  while (totalBytesRead < BUFF_SIZE) {
    ret = read(connfd, buff + totalBytesRead, BUFF_SIZE - totalBytesRead);
    totalBytesRead += ret;
    if (ret == -1) {
      perror("Failed to read \"HELLO\" from buffer.\n");
      exit(1);
    }
  }

  bzero(buff, BUFF_SIZE);
  strncpy(buff, "ACK", 3);

  // send the client an 'ack' to show that we have received the message
  ret = write(connfd, buff, 3);
  if (ret == -1) {
    perror("Failed to \"ACK\" the non-daemon.\n");
    exit(1);
  }

  bzero(buff, BUFF_SIZE);

  // read the non-daemon's clock
  ret = read(connfd, buff, BUFF_SIZE);
  if (ret == -1) {
    perror("Failed to read clock from non-daemon.\n");
    exit(1);
  }

  int thisClock = atoi(buff);
  bzero(buff, BUFF_SIZE);

  // acquire lock to update global running total
  pthread_mutex_lock(&clockLock);
  newClock += thisClock;
  numClocksReceived++;
  pthread_mutex_unlock(&clockLock);

  // halt execution of this thread until all clocks have been received
  pthread_mutex_lock(&threadLock);
  pthread_mutex_unlock(&threadLock);

  // continue execution of this thread with the new average 
  itoa(newClock, buff);

  ret = write(connfd, buff, BUFF_SIZE);
  if (ret == -1) {
    perror("Failed to \"ACK\" the non-daemon.\n");
    exit(1);
  }

  bzero(buff, BUFF_SIZE);

  ret = read(connfd, buff, BUFF_SIZE);
  if (ret == -1) {
    perror("Failed to read \"BYE\" from non-daemon.\n");
    exit(1);
  }

  if (strncmp(buff, "BYE", 3) == 0) {
    // printf("Non-daemon disconnected.\n");
  } else {
    perror("Received something other than \"BYE\" from non-daemon.\n");
    exit(1);
  }

  return NULL;
}

// if this processes' port number is the same as the daemon's,
// it is the daemon
int isDaemon(int port, int daemonPort) {
  return port == daemonPort;
}

// convert string to int
// dead stolen from https://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux
void itoa(int n, char* s) {
  int i, sign;

  if ((sign = n) < 0)  /* record sign */
     n = -n;          /* make n positive */
  i = 0;
  do {       /* generate digits in reverse order */
     s[i++] = n % 10 + '0';   /* get next digit */
  } while ((n /= 10) > 0);     /* delete it */
  if (sign < 0)
     s[i++] = '-';
  s[i] = '\0';

  int k, j;
  char c;

  for (k = 0, j = strlen(s)-1; k<j; k++, j--) {
    c = s[k];
    s[k] = s[j];
    s[j] = c;
  }
}

// if the machine is not the time daemon, connect to it
void normalInit(int daemonPort, int* logicalClock) {
  int writeTo[2];
  int ret = makeInitSock(daemonPort, writeTo);

  if (ret == 0) {
    int sockfd = writeTo[0];
    int clifd = writeTo[1];
    
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
int normalInteraction(int sockfd, int clifd, int* logicalClock) {
  char buff[BUFF_SIZE];
  bzero(buff, BUFF_SIZE);
  strncpy(buff, "HELLO", 5);

  // send an initial HELLO msg
  int ret = write(sockfd, buff, BUFF_SIZE);
  if (ret == -1) {
    perror("Failed to write hello to buffer.\n");
    return -1;
  }

  bzero(buff, BUFF_SIZE);

  // read the server's ACK to the client's HELLO
  ret = read(sockfd, buff, 3);
  if (ret == -1) {
    perror("Failed to read ack from buffer.\n");
    return -1;
  }

  bzero(buff, BUFF_SIZE);

  // convert the logicalClock of this process to a sendable format
  itoa(*logicalClock, buff);

  // write the clock value over to the daemon
  ret = write(sockfd, buff, BUFF_SIZE);
  if (ret == -1) {
    perror("Failed to write clock to buffer.\n");
    return -1;
  }

  bzero(buff, BUFF_SIZE);

  // receive the new value for the clock
  ret = read(sockfd, buff, BUFF_SIZE);
  if (ret == -1) {
    perror("Failed to read new clock value from buffer.\n");
    return -1;
  }

  // MAY NEED TO CONVERT IT TO (+/-)n INSTEAD OF RAW VALUE
  // convert the new clock value to int and reset this clock
  *logicalClock = atoi(buff);

  bzero(buff, BUFF_SIZE);
  strncpy(buff, "BYE", 3);

  // send "BYE" to close the connection
  ret = write(sockfd, buff, 3);
  if (ret == -1) {
    perror("Failed to write transaction to buffer.\n");
    return -1;
  }

  return 0;
}

