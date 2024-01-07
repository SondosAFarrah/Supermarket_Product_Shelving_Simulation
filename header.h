
#ifndef LIBRARIES
#define LIBRARIES

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <pthread.h>
#include<sys/shm.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#endif
typedef struct {

    pid_t pid;
    float amount;
    int shoppingTime;
    int waitingTime;
    int impatienceThreshold;
    int itemsNum;
    int cashierMsgQueueId;
} Customer;

typedef struct {
    long msgType;
    Customer customerIsWaiting;
} MessageQueue;



typedef struct {
    char name[15];
    int available_number;
} Product;
