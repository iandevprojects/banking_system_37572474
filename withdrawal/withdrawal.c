#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../utilities/utilities.h"

void withdrawAmount()
{
    // Account Number section
    char input[50];
    long accountNumber;

    while (1)
    {
        printf("Enter your account number: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error. Try again.\n\n");
            continue;
        }

        trimWhitespace(input);

        if (strlen(input) == 0)
        {
            printf("Account number cannot be empty.\n\n");
            continue;
        }

        if (!isNumericString(input))
        {
            printf("Account number must contain digits only.\n\n");
            continue;
        }

        accountNumber = atol(input);
        if (accountNumber <= 0)
        {
            printf("Invalid account number.\n\n");
            continue;
        }

        char accountFilePath[256];
        sprintf(accountFilePath, "database/%ld.txt", accountNumber);

        FILE *accFile = fopen(accountFilePath, "r");
        if (!accFile)
        {
            printf("Account does not exist. Try again.\n\n");
            continue;
        }

        fclose(accFile);
        break;
    }

    printf("Account %ld selected.\n\n", accountNumber);

    // PIN Section
    char accountFilePath[256];
    sprintf(accountFilePath, "database/%ld.txt", accountNumber);

    FILE *accFile = fopen(accountFilePath, "r");
    if (!accFile)
    {
        printf("Failed to open account file.\n");
        return;
    }

    char fileLine[256];
    char storedPIN[10] = "";

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

    if (strlen(storedPIN) != 4 || !isNumericString(storedPIN))
    {
        printf("Stored PIN is invalid.\n");
        return;
    }

    char inputPIN[10];

    while (1)
    {
        printf("Enter your 4-digit PIN: ");

        if (fgets(inputPIN, sizeof(inputPIN), stdin) == NULL)
        {
            printf("Input error. Try again.\n\n");
            continue;
        }

        trimWhitespace(inputPIN);

        if (strlen(inputPIN) != 4 || !isNumericString(inputPIN))
        {
            printf("Invalid PIN. PIN must be exactly 4 digits.\n\n");
            continue;
        }

        if (strcmp(inputPIN, storedPIN) != 0)
        {
            printf("Incorrect PIN. Try again.\n\n");
            continue;
        }

        break;
    }

    printf("PIN verified successfully.\n\n");

    // Account Balance section
    accFile = fopen(accountFilePath, "r");
    if (!accFile)
    {
        printf("Failed to read account.\n");
        return;
    }

    double currentBalance = 0.0;

    while (fgets(fileLine, sizeof(fileLine), accFile))
    {
        if (strncmp(fileLine, "Balance:", 8) == 0)
        {
            currentBalance = atof(fileLine + 8);
            break;
        }
    }
    fclose(accFile);

    printf("Available Balance: RM %.2f\n\n", currentBalance);

    // Withdrawal Section
    int withdrawAmount;

    while (1)
    {
        printf("Enter amount to withdraw (RM 1 - RM 50000): ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error. Try again.\n\n");
            continue;
        }

        trimWhitespace(input);

        if (!isNumericString(input))
        {
            printf("Amount must contain digits only.\n\n");
            continue;
        }

        withdrawAmount = atoi(input);

        if (withdrawAmount < 1 || withdrawAmount > 50000)
        {
            printf("Amount must be between RM 1 and RM 50000.\n\n");
            continue;
        }

        if (withdrawAmount > currentBalance)
        {
            printf("Insufficient balance. You only have RM %.2f.\n\n", currentBalance);
            continue;
        }

        break;
    }

    // Confirmation Section
    char confirm[10];

    while (1)
    {
        printf("Confirm withdrawal of RM %d from account %ld? (yes/no): ",
               withdrawAmount, accountNumber);

        if (fgets(confirm, sizeof(confirm), stdin) == NULL)
        {
            printf("Input error. Try again.\n\n");
            continue;
        }

        trimWhitespace(confirm);
        toLowerCase(confirm);

        if (strcmp(confirm, "yes") == 0)
        {
            break;
        }
        else if (strcmp(confirm, "no") == 0)
        {
            printf("\nWithdrawal cancelled.\n\n");
            return;
        }
        else
        {
            printf("Invalid input. Please enter 'yes' or 'no'.\n\n");
        }
    }

    // Update Balance
    char originalPath[256];
    char tempPath[256];

    sprintf(originalPath, "database/%ld.txt", accountNumber);
    sprintf(tempPath, "database/%ld_temp.txt", accountNumber);

    FILE *orig = fopen(originalPath, "r");
    FILE *temp = fopen(tempPath, "w");

    if (!orig || !temp)
    {
        printf("Error opening files for update.\n");
        if (orig)
            fclose(orig);
        if (temp)
            fclose(temp);
        return;
    }

    double newBalance = currentBalance - withdrawAmount;

    while (fgets(fileLine, sizeof(fileLine), orig))
    {
        if (strncmp(fileLine, "Balance:", 8) == 0)
        {
            fprintf(temp, "Balance: %.2f\n", newBalance);
        }
        else
        {
            fputs(fileLine, temp);
        }
    }

    fclose(orig);
    fclose(temp);

    remove(originalPath);
    rename(tempPath, originalPath);

    printf("\nWithdrawal successful.\n");
    printf("Account Number: %ld\n", accountNumber);
    printf("Remaining Balance: RM %.2f\n\n", newBalance);

    FILE *logFile = fopen("database/log.txt", "a");
    if (!logFile)
    {
        printf("Warning: Could not open log file.\n");
    }
    else
    {
        time_t now;
        struct tm *local;
        char dateTime[100];

        time(&now);
        local = localtime(&now);

        strftime(dateTime, sizeof(dateTime), "%d %B %Y, %I:%M %p", local);

        fprintf(logFile, "[%s] WITHDRAWAL - Account: %ld | Amount: RM %d\n",
        dateTime, accountNumber, withdrawAmount);

        fclose(logFile);
    }
}