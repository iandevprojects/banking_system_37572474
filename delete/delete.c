#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utilities/utilities.h"

void deleteAccount()
{
    // Printing out bank accounts
    FILE *indexFile = fopen("database/index.txt", "r");
    if (!indexFile)
    {
        printf("No accounts found.\n");
        return;
    }

    char line[50];
    long accountNumbers[1000];
    int count = 0;

    printf("===================================\n");
    printf("        Existing Accounts\n");
    printf("===================================\n");

    while (fgets(line, sizeof(line), indexFile))
    {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0)
            continue;

        long accNum = atol(line);
        accountNumbers[count++] = accNum;
        printf("%d. %ld\n", count, accNum);
    }

    fclose(indexFile);

    if (count == 0)
    {
        printf("No accounts available to delete.\n");
        return;
    }

    // Enter choice to select account
    char input[50];
    long selectedAccount;

    while (1)
    {
        printf("\nEnter the number of the account to delete: ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error. Try again.\n");
            continue;
        }

        // Trim Whitespace
        trimWhitespace(input);

        // Check if input is empty
        if (strlen(input) == 0)
        {
            printf("Input cannot be empty.\n");
            continue;
        }

        // Check if numeric
        if (!isNumericString(input))
        {
            printf("Account number must be digits only.\n");
            continue;
        }

        selectedAccount = atol(input);

        int found = 0;
        for (int i = 0; i < count; i++)
        {
            if (accountNumbers[i] == selectedAccount)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            printf("Account number not found. Please enter a valid account number.\n");
            continue;
        }

        break;
    }

    printf("You selected account number: %ld\n", selectedAccount);

    char accountFilePath[256];
    sprintf(accountFilePath, "database/%ld.txt", selectedAccount);

    FILE *accFile = fopen(accountFilePath, "r");
    if (!accFile)
    {
        printf("Failed to open account file.\n");
        return;
    }

    // ID Number section
    char fileLine[256];
    char storedID[50] = "";
    while (fgets(fileLine, sizeof(fileLine), accFile))
    {
        if (strncmp(fileLine, "ID Number:", 10) == 0)
        {
            // Copy the part after the "ID Number: "
            strcpy(storedID, fileLine + 11);
            trimWhitespace(storedID);
            break;
        }
    }

    fclose(accFile);

    if (strlen(storedID) < 4)
    {
        printf("Account ID Number is invalid.\n");
        return;
    }

    // Ask the user for the last 4 digits of the ID Number
    char inputID[10];
    while (1)
    {
        printf("\nEnter the last 4 digits of the account's ID Number: ");
        if (fgets(inputID, sizeof(inputID), stdin) == NULL)
        {
            printf("Input Error. Try again.\n\n");
            continue;
        }

        trimWhitespace(inputID);

        // Check if the input is numberic and exactly 4 digits
        if (!isNumericString(inputID) || strlen(inputID) != 4)
        {
            printf("Invalid input. Input must be 4 digits.\n");
            continue;
        }

        // Compare the last 4 digits
        int storedLen = strlen(storedID);
        if (strcmp(inputID, storedID + storedLen - 4) != 0)
        {
            printf("ID verification failed. Last 4 digits do not match.\n");
            continue;
        }

        break;
    }

    printf("ID verification successful.\n");

    // PIN Section
    char storedPIN[10] = "";
    accFile = fopen(accountFilePath, "r");
    if (!accFile)
    {
        printf("Failed to open account file.\n");
        return;
    }

    // Skip "PIN: " in the file
    while (fgets(fileLine, sizeof(fileLine), accFile))
    {
        if (strncmp(fileLine, "PIN:", 4) == 0)
        {
            strcpy(storedPIN, fileLine + 5);
            trimWhitespace(storedPIN);
            break;
        }
    }
    fclose(accFile);

    if (strlen(storedPIN) != 4)
    {
        printf("Stored PIN is invalid.\n");
        return;
    }

    // Prompt the user for their PIN
    char inputPIN[10];
    while (1)
    {
        printf("\nEnter the 4-digit PIN: ");
        if (fgets(inputPIN, sizeof(inputPIN), stdin) == NULL)
        {
            printf("Input error. Try again.\n\n");
            continue;
        }
        trimWhitespace(inputPIN);

        // Input validation
        if (!isNumericString(inputPIN) || strlen(inputPIN) != 4)
        {
            printf("Invalid PIN. Must be exactly 4 digits\n");
            continue;
        }

        // Compare input to PIN
        if (strcmp(inputPIN, storedPIN) != 0)
        {
            printf("PIN verification failed.\n");
            continue;
        }

        break;
    }

    printf("PIN verified successfully.\n\n");

    // Prompt the user for a final confirmation
    char confirm[10];
    while (1)
    {
        printf("Are you sure you want to delete account %ld? (yes/no): ", selectedAccount);
        if (fgets(confirm, sizeof(confirm), stdin) == NULL)
        {
            printf("Input error. Try again.\n");
            continue;
        }

        trimWhitespace(confirm);
        toLowerCase(confirm);

        if (strcmp(confirm, "yes") == 0)
        {
            break; // proceed with deletion
        }
        else if (strcmp(confirm, "no") == 0)
        {
            printf("\nAccount deletion cancelled.\n\n");
            return; // exit function
        }
        else
        {
            printf("Invalid input. Please enter 'yes' or 'no'.\n\n");
            continue;
        }
    }

    printf("Confirmed. Continuing with deletion...\n");

    // Updating the database
    if (remove(accountFilePath) != 0)
    {
        printf("Failed to delete account file.\n");
        return;
    }

    FILE *tempFile = fopen("database/index_temp.txt", "w");
    if (!tempFile)
    {
        printf("Failed to open temp file.\n");
        return;
    }

    indexFile = fopen("database/index.txt", "r");
    if (!indexFile)
    {
        printf("Failed to open index file.\n");
        fclose(tempFile);
        return;
    }

    while (fgets(line, sizeof(line), indexFile))
    {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0)
            continue;

        long accNum = atol(line);
        if (accNum != selectedAccount)
        {
            fprintf(tempFile, "%ld\n", accNum);
        }
    }

    fclose(indexFile);
    fclose(tempFile);

    remove("database/index.txt");
    rename("database/index_temp.txt", "database/index.txt");

    printf("\nAccount has successfully been deleted.\n\n");
}