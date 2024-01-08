#include "header.h"
 void customerShoppingProcess(Product* sharedData, int customerId, int minTh) {

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
        if(sharedData[itemIndex].available_number <= minTh){

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
void main(int argc, char *argv[]){
    int Cid = atoi(argv[1]);
    int minTh =atoi(argv[2]);
    key_t key1 = ftok(".", 'm');
    int shm_id2 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);
    Product* sharedData =  (Product *) shmat(shm_id2, NULL, 0);

    customerShoppingProcess(sharedData,Cid,minTh);

}
