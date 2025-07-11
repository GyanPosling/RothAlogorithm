#include"validators.h"
#include <stdio.h>

int getValidInput(int min, int max) {
    int num;
    char symbol;

    while (1) {
        char buffer[100];

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input!\nTry again: ");
            continue;
        }

        if (sscanf(buffer, "%d %c", &num, &symbol) != 1) {
            printf("Invalid input! Please enter a number.\nTry again: ");
            continue;
        }

        if (num < min || num > max) {
            printf("The number is not within the range [%d, %d]!\nTry again: ", min, max);
            continue;
        }

        return num;
    }
}
