#include "header.h"
int shm_id3, shm_id2, shm_id1;
void checkingTheStorage(Product* storageArea);
void main(int argc, char *argv[]){
    // opening the storageArea
    shm_id1 = atoi(argv[1]); 
    shm_id2 = atoi(argv[2]); 
    printf("------ Hey Im storage guard!! \n");
    key_t key3 = ftok(".", 's');
     shm_id3 = shmget(key3, SHM_SIZE, IPC_CREAT | 0666);
    Product* storageArea =  (Product *) shmat(shm_id3, NULL, 0);
    checkingTheStorage(storageArea);
}
void checkingTheStorage(Product* storageArea){
    int emptyProducts =0;
    while(1){
        for(int i =0 ; i < numberOfItems ; i++){
            if(storageArea[i].available_number == 0){
                emptyProducts++;
            }
        }
        if(emptyProducts == numberOfItems){
            printf("The Storage Area is out of Stock, Lets close and take holiday!! \n");
            shmctl(shm_id1, IPC_RMID, NULL);
        shmctl(shm_id2, IPC_RMID, NULL);
        shmctl(shm_id3, IPC_RMID, NULL);
       int messageQueueId;
        MessageQueue message;
        messageQueueId = msgget(key,0666 | IPC_CREAT);
        msgctl(messageQueueId, IPC_RMID, NULL);
            kill(0, SIGTERM);
        } else if(emptyProducts != numberOfItems){
            emptyProducts = 0;
        }
    }
}
