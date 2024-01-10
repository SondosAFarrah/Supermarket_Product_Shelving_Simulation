#include "header.h"
void teamProcess(Team* sharedTeams,MessageQueue message);
void*  managerProcess(void* arg);
void*  employeeProcess();
int chosenTeam;
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

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    MessageQueue message;
    chosenTeam= message.teamIndex;
    int msQid = sharedTeams[chosenTeam].msgQueueId;
    message.msgType = msQid;
    printf("WAITING %s\n",message.productToGet.name);
    //msgrcv(msQid, &message, sizeof(message) - sizeof(long), 1, 0);
    printf("AFTER REC %s\n",message.productToGet.name);
    //teamProcess(sharedTeams, message);
    ssize_t receivedBytes = msgrcv(msQid, &message, sizeof(message) - sizeof(long), 1, 0);
    if (receivedBytes == -1) {
        perror("Error receiving message");
        // You can handle the error in an appropriate way, such as exiting the program or trying again.
        // For now, we'll just print an error message and continue.
    } else {
        printf("Message received successfully, %zd bytes\n", receivedBytes);
        // Process the received message or call other functions as needed
        teamProcess(sharedTeams, message);
    }
    printf("RETURNINNNG AND FINISHING \n");
}
void teamProcess(Team* sharedTeams,MessageQueue message){
    printf("SSSSSSSSSSSSSIGNAL\n");
//    int* chosenTeam = (int*)malloc(sizeof(int));
//    *chosenTeam = rand() % 3;

    printf("********************************* %d\n",chosenTeam);
    printf("TEAM %d IS CHOOOOOOOOSEN\n",sharedTeams[chosenTeam].team_id);
    if(pthread_create(&sharedTeams[chosenTeam].manager,NULL,managerProcess,(void*)&message)!=0){
        perror("pthread create:");
        exit(EXIT_FAILURE);
    }
    pthread_join(sharedTeams[chosenTeam].manager, NULL);
    int array = sizeof(sharedTeams[chosenTeam].employee) / sizeof(sharedTeams[chosenTeam].employee[0]);
    printf("ARRAY SIZE IS %d\n",array);
    for (int i = 0; i < array; ++i) {
        printf("IM IN FOR LOOOOP\n");
        if(pthread_create(&sharedTeams[chosenTeam].employee[i],NULL,employeeProcess,NULL)!=0){
            perror("pthread create:");
            exit(EXIT_FAILURE);
        }
        printf("IM IN FOR LOOOOP\n");
    }

}
void* managerProcess(void* arg){
    MessageQueue* message =(MessageQueue*) arg;
    printf("IM IN MANAGER PROCESSSS, AND THE PRODUCT IS %s \n",message->productToGet.name);
    key_t key1 = ftok(".", 's');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* sharedData =  (Product *) shmat(shm_id1, NULL, 0);
    key_t key2 = ftok(".", 'j');
    int shm_id2 = shmget(key2, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id2, NULL, 0);

    printf("MANAGER IS WORKING\n");
    sleep(2);
    printf("FINISHING\n");
}
void* employeeProcess(){
    printf("HELLO IM THREAD DOING SOMTHING!!!!\n");
}