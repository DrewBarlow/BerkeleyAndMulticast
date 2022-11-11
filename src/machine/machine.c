#include "machine.h"

// accept a new client on the socket
int acceptClient(int sockfd, struct sockaddr_in* serveradd) {
  int addrlen = sizeof(*serveradd);
  int connfd = accept(sockfd, (struct sockaddr*)serveradd, (socklen_t*)&addrlen);

  // 0 success, else err
  if (connfd < 0) {
    perror("Client connected, but something went wrong.\n");
    return -1;
  } else { printf("Client connected.\n"); }

  return connfd;
}

// retrieve resp and init port numbers, along with the number of clients
void getArgs(int argc, char** argv, int* writeTo) {
  if (!isEnoughArgs(argc)) { exit(1); }

  // resp and init port number, respectively
  int iArg1 = getPortNumber(argv[1]);
  int iArg2 = getPortNumber(argv[2]);

  // num machines in the system
  int iArg3 = atoi(argv[3]);
  if (iArg3 <= 0) {
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

  perror("Please enter these args: <portNumber> <numClients>\n");
  return 0;
}

// create a socket specifically for listening
int makeRespSock(int port, int numClients, int* sockfd, struct sockaddr_in* serveradd) {
  // required for sock definition
  *sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // 0 err, >=0 success
  if (*sockfd == 0) {
    perror("Error instantiating socket.\n");
    return -1;
  }

  // fill the structure with zeros
  bzero(serveradd, sizeof(struct sockaddr_in));

  // set up IP/Port
  serveradd->sin_family = AF_INET;
  serveradd->sin_addr.s_addr = htonl(INADDR_ANY); 
  serveradd->sin_port = htons(port);

  // bind info to socket
  int bindRet = bind(*sockfd, (struct sockaddr*)serveradd, sizeof(struct sockaddr_in));

  // 0 success, else err
  if (bindRet != 0) {
    perror("Error binding socket.\n");
    return -1;
  } else { printf("Successfully bound socket.\n"); }

  int listenRet = listen(*sockfd, numClients);
  if (listenRet != 0) {
    perror("Failed to listen.\n");
    return -1;
  } else { printf("Listening on port %d.\n", port); }

  return 0;
}

// create a socket specifically for sending
int makeInitSock(int port, int* writeTo) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // -1 err, >=0 success
  if (sockfd == -1) {
    perror("Error instantiating socket.\n");
    return -1;
  }

  // create a structure and fill it with zeros
  struct sockaddr_in serveradd;
  bzero(&serveradd, sizeof(struct sockaddr_in));

  // set up IP/Port
  serveradd.sin_family = AF_INET;
  serveradd.sin_addr.s_addr = htonl(INADDR_ANY);  // usually pass in an IP here
  serveradd.sin_port = htons(port);

  // connect to the server
  int clifd = connect(sockfd, (struct sockaddr*)&serveradd, sizeof(struct sockaddr_in));
  if (clifd < 0) {
    perror("Failed to connect.\n");
    return -1;
  }
  
  writeTo[0] = sockfd;
  writeTo[1] = clifd;

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

