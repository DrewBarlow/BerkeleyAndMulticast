#include "socks.h"

// accept a new client on the socket
int acceptClient(int sockfd, struct sockaddr_in* serveradd) {
  int addrlen = sizeof(*serveradd);
  int connfd = accept(sockfd, (struct sockaddr*)serveradd, (socklen_t*)&addrlen);

  // 0 success, else err
  if (connfd < 0) {
    perror("Connection happened, but something went wrong.\n");
    return -1;
  }

  return connfd;
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
  }

  int listenRet = listen(*sockfd, numClients);
  if (listenRet != 0) {
    perror("Failed to listen.\n");
    return -1;
  }

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

