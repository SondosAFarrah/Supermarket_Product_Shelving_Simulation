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
int managerDecesion = 0;
void main(int argc, char *argv[]){
    //team shared
    key_t key1 = ftok(".", 'j');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id1, NULL, 0);
    int messageQueueId;
    MessageQueue message;
    if((messageQueueId = msgget(key,0666 | IPC_CREAT)) == -1){
        perror("msgget ");
        exit(EXIT_FAILURE);
    }
    while(msgrcv(messageQueueId,&message,sizeof(message) - sizeof(long), message.msgType,0) != -1){
        chosenTeam= message.teamIndex;
        teamProcess(sharedTeams, message);
    }
}
void teamProcess(Team* sharedTeams,MessageQueue message){
    printf("im in team %d \n",getpid());
    teamID = sharedTeams[chosenTeam].team_id;
    key_t key1 = ftok(".", 'm');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* shelveArea =  (Product *) shmat(shm_id1, NULL, 0);
    if(pthread_create(&sharedTeams[chosenTeam].manager,NULL,managerProcess,(void*)&message)!=0){
        perror("pthread create:");
        exit(EXIT_FAILURE);
    }
    pthread_join(sharedTeams[chosenTeam].manager, NULL);
    if(managerDecesion == 0) {
        int array = sizeof(sharedTeams[chosenTeam].employee) / sizeof(sharedTeams[chosenTeam].employee[0]);
        for (int i = 0; i < array; ++i) {
            if (pthread_create(&sharedTeams[chosenTeam].employee[i], NULL, employeeProcess, NULL) != 0) {
                perror("pthread create:");
                exit(EXIT_FAILURE);
            }
        }
        // Join employee threads (if needed)
        for (int i = 0; i < array; ++i) {
            pthread_join(sharedTeams[chosenTeam].employee[i], NULL);
        }
        printf("The available number of product %s after filled by a team %d in the shelve is %d  \n",
               shelveArea[in].name, getpid(), shelveArea[in].available_number);
        shelveArea[in].isFree = 0;
    }
}

void* managerProcess(void* arg){
  //  pthread_mutex_lock(&managerMutex);
    MessageQueue* message =(MessageQueue*) arg;
    int index = message->teamIndex;
    //printf("Im the manager of the team with teamId  %d and im going to fill the card wth product  %s \n",teamID,message->productToGet.name);
    key_t key1 = ftok(".", 'm');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* shelveArea =  (Product *) shmat(shm_id1, NULL, 0);
    key_t key3 = ftok(".", 's');
    int shm_id3 = shmget(key3, SHM_SIZE, IPC_CREAT | 0666);
    Product* storageArea =  (Product *) shmat(shm_id3, NULL, 0); // here was shm_id1 is passed !!! why??
    key_t key2 = ftok(".", 'j');
    int shm_id2 = shmget(key2, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id2, NULL, 0);
    for (int i = 0; i < 3; ++i) {
        if (strcmp(shelveArea[i].name, message->productToGet.name) == 0){
            in=i;
        }
    }
    neededProduct = 5 - message->productToGet.available_number;
    if(storageArea[in].available_number >= neededProduct) {
        printf("manager of team %d is filling the card with product %s \n", getpid(), message->productToGet.name);

        storageArea[in].available_number -= neededProduct;
        sleep(2);
        printf("manager of team %d has filled the card with  a %d items of  product %s and the available in the Storage Area is : %d  \n",
               getpid(), neededProduct, message->productToGet.name, storageArea[in].available_number);
        managerDecesion =0;
    }else if(storageArea[in].available_number < neededProduct && storageArea[in].available_number != 0){
        printf("manager of team %d tried to  fill the card with product %s but he find less amount than that need and took it\n", getpid(), message->productToGet.name);
        managerDecesion = 0;
        neededProduct = storageArea[in].available_number;
        storageArea[in].available_number -= neededProduct;
    }
    else if(storageArea[in].available_number == 0){
        printf("manager of team %d tried to  fill the card with product %s but it was empty!! \n", getpid(), message->productToGet.name);
        managerDecesion =1;
    }
}
void* employeeProcess(){
    key_t key1 = ftok(".", 'm');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* shelveArea =  (Product *) shmat(shm_id1, NULL, 0);
   pthread_mutex_lock(&employeeMutex);
    if(neededProduct > 0) {
        shelveArea[in].available_number++;
        neededProduct--;
        printf("An Employee from Team %d has put one item of product %s in the shelve and the available number now: %d  \n",getpid(),shelveArea[in].name,shelveArea[in].available_number);
    }
   pthread_mutex_unlock(&employeeMutex);
    return NULL;
}
