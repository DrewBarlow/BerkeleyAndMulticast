#ifndef __SOCKS_H__
#define __SOCKS_H__

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int acceptClient(int sockfd, struct sockaddr_in* serveradd);
int makeInitSock(int port, int numClients, int* sockfd, struct sockaddr_in* serveradd);
int makeRespSock(int port, int* writeTo);

#endif
