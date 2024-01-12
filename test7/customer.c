#include "header.h"
void chooseTeam(Team* sharedTeams,Product sharedData);
void customerShoppingProcess(Product* sharedData, int customerId, int minTh,Team* sharedTeams);
void stopped(int current_time);
int chosenTeam;
time_t current_time,last_time;
void main(int argc, char *argv[]){
    int Cid = atoi(argv[1]);
    int minTh =atoi(argv[2]);
     current_time = atoi(argv[3]);
    //shelvesShared
    key_t key1 = ftok(".", 'm');
    int shm_id1 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* sharedData =  (Product *) shmat(shm_id1, NULL, 0);
    //teamsShared
    key_t key2 = ftok(".", 'j');
    int shm_id2 = shmget(key2, SHM_SIZE, IPC_CREAT | 0666);
    Team * sharedTeams =  (Team *) shmat(shm_id2, NULL, 0);
    customerShoppingProcess(sharedData,Cid,minTh,sharedTeams);
    stopped(current_time);

}
void customerShoppingProcess(Product* sharedData, int customerId, int minTh,Team* sharedTeams) {
    stopped(current_time);
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
    //sleep(customer.shoppingTime);
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
            printf("IM IN IF \n");
            printf("The first teamId from a customer page is : %d \n",sharedTeams[0].team_id);
            sharedData[itemIndex].isFree=1;
            chooseTeam(sharedTeams,sharedData[itemIndex]);
            printf("IM IN END OF ID \n");

        }
        if (sharedData[itemIndex].available_number >= quantity) {
            // if there is enough number of the specific items then decrement it.
            //printf("Hey there\n");
            sharedData[itemIndex].available_number -= quantity;
            customer.itemsNum+=quantity;
            printf("Customer %d took %d units of item %s\n",customer.pid,quantity,sharedData[itemIndex].name);
            //sleep(5);
            // now we will update the total amount of customer spent.
        } else {
            printf("Customer %d wanted %d units of item %s, but it's not available\n",customer.pid,quantity,sharedData[itemIndex].name);
        }
    }
    stopped(current_time);
}
void chooseTeam(Team* sharedTeams,Product sharedData){

    stopped(current_time);
    // printf("The first index of the teams ID 1: ----- is %d \n ",sharedTeams[0].team_id);
    srand(time(0));
    chosenTeam =rand()%3 ;

    MessageQueue message;
    message.teamIndex=chosenTeam;
    int msQid = sharedTeams[chosenTeam].msgQueueId;
    message.msgType = 1;
    message.productToGet = sharedData;
    printf("SENDING MESSAGE\n");
    if (msgsnd(msQid, &message, sizeof(message) - sizeof(long), 0) == -1) {
        perror("Error sending message");
    } else {
        printf("Message sent successfully with the Product %s \n",message.productToGet.name);
    }
    //printf("STOPED SENDING MESSAGE\n");
    stopped(current_time);
}
void stopped(int current_time){
    if((time(&last_time)-current_time)>=30){
        printf("stopped in teams\n");
        kill(0,SIGTERM);
    }
}
