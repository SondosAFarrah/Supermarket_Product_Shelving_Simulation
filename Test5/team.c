#include "header.h"
void teamProcess(Team* sharedTeams,MessageQueue message);
void*  managerProcess(void* arg);
void*  employeeProcess();
int chosenTeam;
int teamID;
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

}
void* managerProcess(void* arg){
    MessageQueue* message =(MessageQueue*) arg;
    int index = message->teamIndex;
    printf("Im the manager of the team with teamId  %d and im going to fill the card wth product  %s \n",teamID,message->productToGet.name);
    key_t key1 = ftok(".", 's');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* sharedData =  (Product *) shmat(shm_id1, NULL, 0);
    key_t key2 = ftok(".", 'j');
    int shm_id2 = shmget(key2, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id2, NULL, 0);

    printf("manager of team %d is filling the card with product %s \n", message->teamIndex,message->productToGet.name);
    sleep(2);

  //return NULL;
}
void* employeeProcess(){
    printf("HELLO IM THREAD DOING SOMTHING!!!!\n");
 return NULL;
}