#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file = fopen("userDefinedRange.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int teamsNum, employeeNum, shelvesNum, minItemsThreshold, maxItemsThreshold;
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
    printf("teamsNum: %d\n", teamsNum);
    printf("employeeNum: %d\n", employeeNum);
    printf("shelvesNum: %d\n", shelvesNum);
    printf("minItemsThreshold: %d\n", minItemsThreshold);
    printf("maxItemsThreshold: %d\n", maxItemsThreshold);

    // You can now use these variables in your program as needed.

    return 0;
}
