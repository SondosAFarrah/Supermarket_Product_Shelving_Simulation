#include "header.h"
void teamProcess(Team* sharedTeams,MessageQueue message);
void*  managerProcess(void* arg);
void*  employeeProcess();
int chosenTeam;
int teamID;
int neededProduct;
int in;
pthread_mutex_t employeeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t managerMutex = PTHREAD_MUTEX_INITIALIZER;

void main(int argc, char *argv[]){
    //team shared
    key_t key1 = ftok(".", 'j');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id1, NULL, 0);


//    //storage shared
//    key_t key2 = ftok(".", 's');
//    int shm_id2 = shmget(key2, SHM_SIZE, IPC_CREAT | 0666);
//    Product * storageArea =  (Product *) shmat(shm_id2, NULL, 0);
//
//    //Shelve shared
//    key_t key3 = ftok(".", 'm');
//    int shm_id3 = shmget(key3, SHM_SIZE, IPC_CREAT | 0666);
//    Product * shelveArea =  (Product *) shmat(shm_id3, NULL, 0);

    //   printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    MessageQueue message;
    int msQid = sharedTeams[chosenTeam].msgQueueId;
    message.msgType = msQid;
    msgrcv(msQid, &message, sizeof(message) - sizeof(long), 1, 0);
    chosenTeam= message.teamIndex;
    // int msQid = sharedTeams[chosenTeam].msgQueueId;
    //  message.msgType = msQid;
    printf("WAITING %s\n",message.productToGet.name);

    printf("AFTER REC %s\n",message.productToGet.name);






    teamProcess(sharedTeams, message);
    //}
    //printf("RETURNINNNG AND FINISHING \n");
}
void teamProcess(Team* sharedTeams,MessageQueue message){
    teamID = sharedTeams[chosenTeam].team_id;
    key_t key1 = ftok(".", 'm');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* shelveArea =  (Product *) shmat(shm_id1, NULL, 0);
    if(pthread_create(&sharedTeams[chosenTeam].manager,NULL,managerProcess,(void*)&message)!=0){
        perror("pthread create:");
        exit(EXIT_FAILURE);
    }

    pthread_join(sharedTeams[chosenTeam].manager, NULL);

    printf("manager thred is joined 0000000000000000000000");
    int array = sizeof(sharedTeams[chosenTeam].employee) / sizeof(sharedTeams[chosenTeam].employee[0]);
    printf("ARRAY SIZE IS %d\n",array);
    for (int i = 0; i < array; ++i) {
        if(pthread_create(&sharedTeams[chosenTeam].employee[i],NULL,employeeProcess,NULL)!=0){
            perror("pthread create:");
            exit(EXIT_FAILURE);
        }

    }
    // Join employee threads (if needed)
    for (int i = 0; i < array; ++i) {
        pthread_join(sharedTeams[chosenTeam].employee[i], NULL);
    }
    printf("The available number of product %s after filled by a team %d in the shelve is %d  \n",shelveArea[in].name,getpid(),shelveArea[in].available_number);
    shelveArea[in].isFree =0;

}
void* managerProcess(void* arg){
    pthread_mutex_lock(&managerMutex);
    MessageQueue* message =(MessageQueue*) arg;
    int index = message->teamIndex;
    printf("Im the manager of the team with teamId  %d and im going to fill the card wth product  %s \n",teamID,message->productToGet.name);
    key_t key1 = ftok(".", 'm');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* shelveArea =  (Product *) shmat(shm_id1, NULL, 0);
    key_t key3 = ftok(".", 's');
    int shm_id3 = shmget(key3, SHM_SIZE, IPC_CREAT | 0666);
    Product* storageArea =  (Product *) shmat(shm_id3, NULL, 0); // here was shm_id1 is passed !!! why??
    key_t key2 = ftok(".", 'j');
    int shm_id2 = shmget(key2, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id2, NULL, 0);


    //printf("++++++++++++++++++++++++++ %s %d\n",shelveArea[0].name, shelveArea[0].available_number);
    for (int i = 0; i < 3; ++i) {
        if (strcmp(shelveArea[i].name, message->productToGet.name) == 0){
            printf("product ++++++++ %s\n",message->productToGet.name);
           // shelveArea[i].isFree=0;

            in=i;
        }
    }
    printf("manager of team %d is filling the card with product %s \n", getpid(),message->productToGet.name);
    neededProduct=5 - message->productToGet.available_number;
    storageArea[in].available_number-=neededProduct;
    sleep(2);
    printf("manager of team %d has filled the card with  a %d items of  product %s and the available in the Storage Area is : %d  \n", getpid(),neededProduct,message->productToGet.name,storageArea[in].available_number);
    //return NULL;
    pthread_mutex_unlock(&managerMutex);
}
void* employeeProcess(){
    key_t key1 = ftok(".", 'm');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* shelveArea =  (Product *) shmat(shm_id1, NULL, 0);
    printf("the available is : --------------------- %d -----\n",shelveArea[in].available_number);

   pthread_mutex_lock(&employeeMutex);

    if(neededProduct > 0) {
        printf("HELLO IM THREAD DOING SOMTHING!!!!\n");
        shelveArea[in].available_number++;
        neededProduct--;
        printf("An Employee from Team %d has put one item of product %s in the shelve and the available number now: %d  \n",getpid(),shelveArea[in].name,shelveArea[in].available_number);
    }
   pthread_mutex_unlock(&employeeMutex);

    return NULL;
}
