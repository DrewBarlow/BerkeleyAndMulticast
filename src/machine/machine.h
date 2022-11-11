#ifndef __MACHINE_H__
#define __MACHINE_H__

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../util/berkeley/berkeley.h"

#define BUFF_SIZE 1024
#define CLOCK_RANDOM_CEILING 783
#define DEFAULT_LINE_SIZE 512
#define NUM_EXPECTED_ARGS 4
#define MAX_PORT_NUMBER 65535

// may not need to use this
extern pthread_mutex_t lock;

void getArgs(int argc, char** argv, int* writeTo);
int getPortNumber(char* cArg);

// may need more args
void* interaction(void* arg);

int isEnoughArgs(int argc);
void printBuff(char* buff, int size);

#endif
