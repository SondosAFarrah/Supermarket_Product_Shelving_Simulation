
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

#define BUFFER_SIZE 100
#define SHM_SIZE sizeof(Product) * BUFFER_SIZE
typedef struct {

    pid_t pid;
    int shoppingTime;
    int itemsNum;
    int cashierMsgQueueId;
} Customer;

typedef struct {
    char name[15];
    int available_number;
    int isFree;//0 free , 1 busy
} Product;

typedef struct {
    long msgType;
    Product productToGet;
    int teamIndex;
} MessageQueue;

typedef struct {
    pid_t team_id;
    pthread_t manager;
    pthread_t employee[3];
    MessageQueue productsReq;
    int msgQueueId;
} Team;


typedef struct {
    int value;
}threadData;