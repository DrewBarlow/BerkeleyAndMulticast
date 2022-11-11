#ifndef __MACHINE_H__
#define __MACHINE_H__

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define CLOCK_RANDOM_CEILING 783
#define DEFAULT_LINE_SIZE 512
#define NUM_EXPECTED_ARGS 4
#define MAX_PORT_NUMBER 65535

// may not need to use this
extern pthread_mutex_t lock;

extern int logicalClock;

int acceptClient(int sockfd, struct sockaddr_in* serveradd);
void berkelySync(int respPort, int initPort);
void getArgs(int argc, char** argv, int* writeTo);
void getPortNumber(char* cArg);

// may need more args
void* interaction(void* arg);

int isEnoughArgs(int argc);
int makeInitSock(int port, int numClients, int* sockfd, struct sockaddr_in* serveradd);
int makeRespSock(int port, int* writeTo);
void printBuff(char* buff, int size);

#endif
