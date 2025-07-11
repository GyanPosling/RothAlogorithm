#include "rota_minimization/performRota.h"
#include "convert/performConvert.h"
#include "ariphmetics_operations/performCalculation.h"
#include "validators.h"

void printMenu() {
    printf("\n");
    printf("+-----------------------------------+\n");
    printf("|            MAIN MENU              |\n");
    printf("+-----------------------------------+\n");
    printf("|                                   |\n");
    printf("|   1. Perform Convert Operation    |\n");
    printf("|   2. Perform Calculation          |\n");
    printf("|   3. Perform Rota Minimization    |\n");
    printf("|   4. Exit Program                 |\n");
    printf("|                                   |\n");
    printf("+-----------------------------------+\n");
    printf("\nEnter your choice (1-4): ");
}

int main(void) {
    int choice;
    
    do {
        printMenu();
        choice = getValidInput(1, 4);
        
        switch(choice) {
            case 1:
                performConvert();
                break;
            case 2:
                performCalculation();
                break;
            case 3:
                performRota();
                break;
            case 4:
                printf("\n=== Exiting Program... Goodbye! ===\n");
                break;
        }
    } while(choice != 4);

    return 0;
}