#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int item_count, team_count;
void readingUserDefined();
Product* storageSharedMemory();
Product* shelveSharedMemory();
Team* teamsSharedMemory();
int teamsNum , employeeNum, shelvesNum, minItemsThreshold, maxItemsThreshold;
int main() {
    // User Defined Values reading from text file.
    readingUserDefined();
    //creating and reading shared memories for the hole storage and for shelves
    Product* storageArea = storageSharedMemory();
    Product* shelveArea = shelveSharedMemory();
    printf("---------------------WELCOME-----------------------\n\n");
    printf("------------------items in storage--------------------------\n");
    for(int i=0; i< item_count;i++){
        printf("%s %d\n",storageArea[i].name, storageArea[i].available_number);
    }
    printf("---------------------items in shelves--------------------------\n");
    for(int i=0; i< item_count;i++){
        printf("shelve %d: %s %d\n",i+1,shelveArea[i].name, shelveArea[i].available_number);
    }
    // ------------------------------team process----------------------------------------//

    Team *teamArea =teamsSharedMemory();
    printf("---------------------teams info--------------------------\n");
    for(int i=0;i<teamsNum;i++){
        pid_t pid = fork();
        if (pid == 0) {
         //   printf("---test from fork ---- the pid of team is %d \n",pid);
            pid_t p = getpid();
            teamArea[i].team_id= p;
            printf("team %d\n",teamArea[i].team_id);
            teamArea[i].msgQueueId=msgget(IPC_PRIVATE, IPC_CREAT | 0666);
//        if(sharedTeams[i].msgQueueId == -1){
//            perror("msgget");
//            exit(-1);

            execlp("./team", "team", NULL);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            exit(EXIT_FAILURE);
        }
    }
//    printf("---------------------teams info--------------------------\n");
//    for(int i=0; i< teamsNum;i++){
//        printf("team %d\n",teamArea[i].team_id);
//    }
   // printf("---------------------------------------------------------\n");
    // ------------------- Main Loop of customers ----------------------------------------//
    while(1){
        int customersComingNow = rand() % 3 + 1;
        for (int i = 0; i < customersComingNow; i++) {
            pid_t pid = fork();

            if (pid == 0) {
                char  cPID[20],minTh[20];
                sprintf(cPID,"%d",(int) getpid());
                //printf("------------------------------------%d\n",getpid());
                sprintf(minTh,"%d",(int) minItemsThreshold);
                execlp("./customer", "customer",cPID,minTh, NULL);
                exit(EXIT_SUCCESS);
            } else if (pid < 0) {
                exit(EXIT_FAILURE);
            }
        }

        // int randomTime = rand() % 6 + 20;
        sleep(rand() % 6 + 10);
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
    }



    return 0;
}



void readingUserDefined() {
    FILE *file = fopen("userDefinedRange.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
    }

    char line[100]; // Assuming a reasonable line size

    // Read and process each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, " ");
        if (token != NULL) {
            // Get the second token (column) of each row
            token = strtok(NULL, " ");

            // Convert the token to an integer
            int value = atoi(token);

            // Store the value in the appropriate variable based on the first token
            if (strcmp(line, "teamsNum") == 0) {
                teamsNum = value;
            } else if (strcmp(line, "employeeNum") == 0) {
                employeeNum = value;
            } else if (strcmp(line, "shelvesNum") == 0) {
                shelvesNum = value;
            } else if (strcmp(line, "minItemsThreshold") == 0) {
                minItemsThreshold = value;
            } else if (strcmp(line, "maxItemsThreshold") == 0) {
                maxItemsThreshold = value;
            }
        }
    }
    // Close the file after reading
    fclose(file);
    // Display the values (optional)

    // You can now use these variables in y
}
Product* storageSharedMemory() {
    // Get shared memory segment key
    key_t key = ftok(".", 's');
    if (key == -1) {
        perror("ftok");
        exit(-1);
    }

    // Allocate shared memory segment
    int shm_id1 = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    // cashiersSMID = shm_id;
    if (shm_id1 == -1) {
        perror("shmget");
        exit(-1);
    }

    // Attach shared memory segment
    Product *product_buffer = (Product *) shmat(shm_id1, NULL, 0);
    if (product_buffer == (Product * ) - 1) {
        perror("shmat");
        exit(-1);
    }
    // read file
    FILE *file = fopen("products.txt", "r");
    if (file == NULL) {
        perror("fopen");
        exit(-1);
    }

    // Read items from file
    item_count=-1;
    while (!feof(file)) {


        // Read item data
        Product product;
        fscanf(file, "%s %d %d", product.name, &product.available_number, &product.isFree);
        memcpy(&product_buffer[item_count], &product, sizeof(Product));
        item_count++;

    }
    printf("iitemmm count %d\n",item_count);
    fclose(file);
    return product_buffer;
}
Product* shelveSharedMemory(){
    // Get shared memory segment key
    key_t key1 = ftok(".", 'm');
    if (key1 == -1) {
        perror("ftok");
        exit(-1);
    }

    // Allocate shared memory segment
    int shm_id2 = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);

    if (shm_id2 == -1) {
        perror("shmget");
        exit(-1);
    }

    // Attach shared memory segment
    Product *product_buffer = (Product *) shmat(shm_id2, NULL, 0);
    if (product_buffer == (Product * ) - 1) {
        perror("shmat");
        exit(-1);
    }
    // read file
    FILE *file = fopen("products.txt", "r");
    if (file == NULL) {
        perror("fopen");
        exit(-1);
    }

    // Read items from file
    item_count=-1;
    while (!feof(file)) {
        item_count++;

        // Read item data
        Product product;

        fscanf(file, "%s %d %d", product.name, &product.available_number,&product.isFree);
        product.available_number=maxItemsThreshold-5;
        memcpy(&product_buffer[item_count], &product, sizeof(Product));

    }

    fclose(file);
    return product_buffer;
}
Team* teamsSharedMemory() {
    // Get shared memory segment key
    key_t key = ftok(".", 'j');
    if (key == -1) {
        perror("ftok");
        exit(-1);
    }
    // Allocate shared memory segment
    int shm_id3 = shmget(key, SHM_SIZE, IPC_CREAT | 0666);

    if (shm_id3 == -1) {
        perror("shmget");
        exit(-1);
    }
    // Attach shared memory segment
    Team * team_buffer = (Team *) shmat(shm_id3, NULL, 0);
    if (team_buffer == (Team * ) - 1) {
        perror("shmat");
        exit(-1);
    }

    return team_buffer;
}
