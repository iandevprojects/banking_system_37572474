#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "menu/menu.h"

void sessionInfo() {
    time_t now;
    struct tm *local;
    char dateTime[100];
    int totalAccounts = 0; // Add the count accounts soon

    time(&now);
    local = localtime(&now);
    strftime(dateTime, sizeof(dateTime), "%d %B %Y, %I:%M %p", local);

    printf("===================================\n\n");
    printf("   BANKING SYSTEM SESSION INFO   \n\n");
    printf("===================================\n\n");
    printf("Session Time: %s\n", dateTime);
    printf("Total Active Accounts: %d\n\n", totalAccounts);
    printf("===================================\n\n");
}

int main() {
    char input[50];

    sessionInfo();

    while (1) {
        showMenu();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error!\n");
            continue;
        }
        input[strcspn(input, "\n")] = 0;
        handleMenuChoice(input);
    }

    return 0;
}