#include "header.h"
void checkingTheStorage(Product* storageArea);
void main(int argc, char *argv[]){
    // opening the storageArea
    printf("------ Hey Im storage guard!! \n");
    key_t key3 = ftok(".", 's');
    int shm_id3 = shmget(key3, SHM_SIZE, IPC_CREAT | 0666);
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
            kill(0, SIGTERM);
        } else if(emptyProducts != numberOfItems){
            emptyProducts = 0;
        }
    }
}
