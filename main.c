#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#define BUFFER_SIZE 100
#define SHM_SIZE sizeof(Product) * BUFFER_SIZE
int item_count;
void readingUserDefined();
Product* storageSharedMemory();
Product* shelveSharedMemory();

int teamsNum , employeeNum, shelvesNum, minItemsThreshold, maxItemsThreshold;
int main() {

    // User Defined Values reading from text file.
    readingUserDefined();
    //creating and reading shared memories for the hole storage and for shelves
    Product* storageArea = storageSharedMemory();
    Product* shelveArea = shelveSharedMemory();
    for(int i=0; i< item_count;i++){
        printf("%s %d\n",storageArea[i].name, storageArea[i].available_number);
    }
    printf("---------------------------------------------------------\n");
    for(int i=0; i< item_count;i++){
        printf("%s %d\n",shelveArea[i].name, shelveArea[i].available_number);
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
    int shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    // cashiersSMID = shm_id;
    if (shm_id == -1) {
        perror("shmget");
        exit(-1);
    }

    // Attach shared memory segment
    Product *product_buffer = (Product *) shmat(shm_id, NULL, 0);
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
        fscanf(file, "%s %d", product.name, &product.available_number);
        memcpy(&product_buffer[item_count], &product, sizeof(Product));


    }
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
    int shm_id = shmget(key1, SHM_SIZE, IPC_CREAT | 0666);

    if (shm_id == -1) {
        perror("shmget");
        exit(-1);
    }

    // Attach shared memory segment
    Product *product_buffer = (Product *) shmat(shm_id, NULL, 0);
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

        fscanf(file, "%s %d", product.name, &product.available_number);
        product.available_number=maxItemsThreshold;
        memcpy(&product_buffer[item_count], &product, sizeof(Product));

    }

    fclose(file);
    return product_buffer;
}
