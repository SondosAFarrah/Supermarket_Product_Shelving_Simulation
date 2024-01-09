#include "header.h"
void teamProcess(Team* sharedTeams);
void customerShoppingProcess(Product* sharedData, int customerId, int minTh,Team* sharedTeams);
void managerProcess();

void main(int argc, char *argv[]){
    int Cid = atoi(argv[1]);
    int minTh =atoi(argv[2]);
    //shelvesShared
    key_t key1 = ftok(".", 'm');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* sharedData =  (Product *) shmat(shm_id1, NULL, 0);
    //teamsShared
    key_t key2 = ftok(".", 'j');
    int shm_id2 = shmget(key2, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id2, NULL, 0);

    customerShoppingProcess(sharedData,Cid,minTh,sharedTeams);

}
void customerShoppingProcess(Product* sharedData, int customerId, int minTh,Team* sharedTeams) {

    struct timeval tv; // to make the random different for the customers that came in the same time ----------------------- added 11/12/2023 12:48 by makhamreh.
    unsigned int seed = (unsigned int)(tv.tv_sec * 1000000 + tv.tv_usec + customerId);
    srand(seed);
    Customer customer;
    customer.pid = customerId;
    printf("Customer %d\n", customer.pid);
    customer.shoppingTime = 2;
    customer.itemsNum=0;
    printf("Shopping Time : %d\n", customer.shoppingTime);
    // starting simulating the customer shopping process:
    sleep(customer.shoppingTime);
    // selecting random items with random quantities.
    int selectedItems[3];
    int selectedQuantities[3];
    int numItems = rand() % 3 + 1; // random items from 1 to 5 ;
    //int x=0;
    int indexes[numItems];
    for (int i =0 ; i < numItems; i++) {
        int index = rand() % 3; // random select an item.

        // lastIndex = 0;

        index = rand() % 3; // random select an item.
        for(int j = 0 ; j < numItems;j++){
            if(indexes[j] == index){
                while(indexes[j] == index){
                    index = rand() % 3;
                }
            }
        }

        indexes[i] = index;
        //x= index;
        selectedItems[i] = index;
        selectedQuantities[i] = rand() % 3 + 1; // random quantity
    }
    for(int i =0; i< numItems; i++) {
        int itemIndex = selectedItems[i];
        int quantity = selectedQuantities[i];
        //printf("Hey\n");
        if(sharedData[itemIndex].available_number <= minTh && sharedData[itemIndex].isFree==0){
            teamProcess(sharedTeams);
        }
        if (sharedData[itemIndex].available_number >= quantity) {
            // if there is enough number of the specific items then decrement it.
            //printf("Hey there\n");
            sharedData[itemIndex].available_number -= quantity;
            customer.itemsNum+=quantity;
            printf("Customer %d took %d units of item %s\n",customer.pid,quantity,sharedData[itemIndex].name);
            sleep(5);
            // now we will update the total amount of customer spent.
        } else {
            printf("Customer %d wanted %d units of item %s, but it's not available\n",customer.pid,quantity,sharedData[itemIndex].name);
        }
    }

}
void teamProcess(Team* sharedTeams){
    int chosenTeam =rand() % 3 + 1;

    printf("//////////////////  %d\n",array);
    if(pthread_create(&sharedTeams[chosenTeam].manager,NULL,managerProcess(),NULL)!=0){
        perror("pthread create:");
        exit(EXIT_FAILURE);
    }

}
void managerProcess(){
    key_t key1 = ftok(".", 's');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* sharedData =  (Product *) shmat(shm_id1, NULL, 0);

    key_t key2 = ftok(".", 'j');
    int shm_id2 = shmget(key2, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id2, NULL, 0);

    int array = sizeof(sharedTeams[chosenTeam].employee) / sizeof(sharedTeams[chosenTeam].employee[0]);
    for (int i = 0; i < array; ++i) {
        if(pthread_create(&sharedTeams[chosenTeam].employee[i],NULL,employeeProcess(),NULL)!=0){
            perror("pthread create:");
            exit(EXIT_FAILURE);
        }
    }
}
