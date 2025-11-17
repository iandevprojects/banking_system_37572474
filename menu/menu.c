#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "menu.h"
#include "../create/create.h"
#include "../utilities/utilities.h"
#include "../delete/delete.h"
#include "../deposit/deposit.h"
#include "../withdrawal/withdrawal.h"
#include "../remittance/remittance.h"

void showMenu()
{
    printf("===================================\n\n");
    printf("1. Create Bank Account\t(create)\n");
    printf("2. Delete Bank Account\t(delete)\n");
    printf("3. Deposit Money\t(deposit)\n");
    printf("4. Withdraw Money\t(withdraw)\n");
    printf("5. Remittance    \t(remittance)\n");
    printf("6. Exit System   \t(exit)\n\n");
    printf("===================================\n\n");
    printf("Enter your choice (number or keyword): ");
}

void handleMenuChoice(char *input)
{
    for (int i = 0; input[i]; i++)
        input[i] = tolower(input[i]);

    if (strcmp(input, "1") == 0 || strcmp(input, "create") == 0)
    {
        printf("\n");
        printf("===================================\n\n");
        printf("        CREATE BANK ACCOUNT\n\n");
        printf("===================================\n\n");
        createAccount();
    }
    else if (strcmp(input, "2") == 0 || strcmp(input, "delete") == 0)
    {
        printf("\n");
        printf("===================================\n\n");
        printf("        DELETE BANK ACCOUNT\n\n");
        printf("===================================\n\n");
        deleteAccount();
    }
    else if (strcmp(input, "3") == 0 || strcmp(input, "deposit") == 0)
    {
        printf("\n");
        printf("===================================\n\n");
        printf("          DEPOSIT MONEY\n\n");
        printf("===================================\n\n");
        depositAmount();
        
    }
    else if (strcmp(input, "4") == 0 || strcmp(input, "withdraw") == 0)
    {
        printf("\n");
        printf("===================================\n\n");
        printf("          WITHDRAW MONEY\n\n");
        printf("===================================\n\n");
        withdrawAmount();
    }
    else if (strcmp(input, "5") == 0 || strcmp(input, "remittance") == 0)
    {
        printf("\n");
        printf("===================================\n\n");
        printf("             REMITTANCE\n\n");
        printf("===================================\n\n");
        remittanceAmount();
    }
    else if (strcmp(input, "6") == 0 || strcmp(input, "exit") == 0)
    {
        printf("Thank you for using this system!\n\n");
        exit(0);
    }
    else
    {
        printf("Invalid choice! Please enter a valid number or keyword.\n\n");
    }
}