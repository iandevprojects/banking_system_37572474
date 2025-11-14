#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "create.h"
#include "../utilities/utilities.h"

// Genearte a unique account number
int generateAccountNumber()
{
    FILE *indexFile;
    long accountNumber;
    int unique;
    char line[20];

    do
    {
        int digits = 7 + rand() % 3;
        accountNumber = 0;
        for (int i = 0; i < digits; i++)
        {
            int digit = (i == 0) ? 1 + rand() % 9 : rand() % 10;
            accountNumber = accountNumber * 10 + digit;
        }

        unique = 1;
        indexFile = fopen("database/index.txt", "r");
        if (indexFile)
        {
            while (fgets(line, sizeof(line), indexFile))
            {
                long existing = atol(line);
                if (existing == accountNumber)
                {
                    unique = 0;
                    break;
                }
            }
            fclose(indexFile);
        }
    } while (!unique);

    return accountNumber;
}

void createAccount()
{
    char name[1024];
    char idNumber[50];
    char accountType[20];
    char input[50];
    char pin[10];
    int accountNumber;

    // Name Input
    while (1)
    {

        printf("Enter Full Name (letters and spaces only): ");

        if (fgets(name, sizeof(name), stdin) == NULL)
        {
            printf("Input error! Try again.\n\n");
            continue;
        }

        // Remove newline
        name[strcspn(name, "\n")] = 0;
        trimWhitespace(name);

        if (strlen(name) == 0)
        {
            printf("Name cannot be empty.\n\n");
            continue;
        }

        if (!isAlphabeticString(name))
        {
            printf("Name must contain only letters and spaces.\n\n");
            continue;
        }

        break;
    }

    printf("Name entered: %s\n\n", name);

    // ID Number section

    while (1)
    {
        printf("Enter ID Number (10-12 digits): ");

        if (fgets(idNumber, sizeof(idNumber), stdin) == NULL)
        {
            printf("Input error! Try again.\n\n");
            continue;
        }

        // Rmove newine and trim spaces
        idNumber[strcspn(idNumber, "\n")] = 0;
        trimWhitespace(idNumber);

        int len = strlen(idNumber);

        if (len < 10 || len > 12)
        {
            printf("ID Number must be between 10 and 12 digits.\n\n");
            continue;
        }

        if (!isNumericString(idNumber))
        {
            printf("ID Number must contain only digits.\n\n");
            continue;
        }

        break;
    }

    printf("ID Number entered: %s\n\n", idNumber);

    // Account Type Section
    while (1)
    {
        printf("Select Account Type:\n");
        printf("1. Savings Account  (savings)\n");
        printf("2. Current Account  (current)\n");
        printf("===================================\n");
        printf("Enter your choice (number or keyword): ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error! Try again.\n\n");
            continue;
        }

        // Remove newline and trim spaces
        input[strcspn(input, "\n")] = 0;
        trimWhitespace(input);

        // Make input lowercase
        toLowerCase(input);

        if (strcmp(input, "1") == 0 || strcmp(input, "savings") == 0)
        {
            strcpy(accountType, "Savings");
            break;
        }
        else if (strcmp(input, "2") == 0 || strcmp(input, "current") == 0)
        {
            strcpy(accountType, "Current");
            break;
        }
        else
        {
            printf("Invalid choice. Please enter a valid number or keyword.\n\n");
        }
    }

    printf("Selected Account Type: %s\n", accountType);

    // PIN Section
    while (1)
    {
        printf("\nEnter a 4-digit PIN: ");

        if (fgets(pin, sizeof(pin), stdin) == NULL)
        {
            printf("Input error. Try again.\n");
            continue;
        }

        // Remove newline and trim spaces
        pin[strcspn(pin, "\n")] = 0;
        trimWhitespace(pin);

        if (strlen(pin) != 4)
        {
            printf("PIN must be exactly 4 digits.\n");
            continue;
        }

        if (!isNumericString(pin))
        {
            printf("PIN must contain only digits.\n");
            continue;
        }

        break;
    }

    printf("PIN set successfully.\n\n");

    // Account Number section
    // Generate account number
    accountNumber = generateAccountNumber();
    printf("Generated Account Number: %d\n\n", accountNumber);

    // Save account number in index.txt
    FILE *indexFile = fopen("database/index.txt", "a");
    if (indexFile)
    {
        fprintf(indexFile, "%d\n", accountNumber);
        fclose(indexFile);
    }
    else
    {
        printf("Warning: Could not update index.txt\n");
    }

    // Saving account to .txt file
    char filePath[256];
    sprintf(filePath, "database/%d.txt", accountNumber);

    FILE *accountFile = fopen(filePath, "w");
    if (!accountFile)
    {
        printf("Failed to create account file.\n\n");
        return;
    }

    fprintf(accountFile, "Name: %s\n", name);
    fprintf(accountFile, "ID Number: %s\n", idNumber);
    fprintf(accountFile, "Account Number: %d\n", accountNumber);
    fprintf(accountFile, "Account Type: %s\n", accountType);
    fprintf(accountFile, "PIN: %s\n", pin);
    fprintf(accountFile, "Balance: %.2f\n", 0.0); // Initial balance

    fclose(accountFile);

    printf("===================================\n\n");
    printf("   ACCOUNT SUCCESSFULLY CREATED\n\n");
    printf("===================================\n\n");
}