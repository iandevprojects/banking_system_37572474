#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utilities/utilities.h"

void remittanceAmount()
{
    // Verification Section
    char input[50];
    long userAcc;

    while (1)
    {
        printf("Enter your account number: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error. Try again.\n\n");
            continue;
        }

        trimWhitespace(input);

        if (!isNumericString(input) || strlen(input) == 0)
        {
            printf("Account number must contain digits only.\n\n");
            continue;
        }

        userAcc = atol(input);
        if (userAcc <= 0)
        {
            printf("Invalid account number.\n\n");
            continue;
        }

        char path[256];
        sprintf(path, "database/%ld.txt", userAcc);

        FILE *acc = fopen(path, "r");
        if (!acc)
        {
            printf("Account does not exist. Try again.\n\n");
            continue;
        }

        fclose(acc);
        break;
    }

    printf("Account %ld selected.\n\n", userAcc);

    // PIN Section
    char userPath[256];
    sprintf(userPath, "database/%ld.txt", userAcc);

    FILE *accFile = fopen(userPath, "r");
    if (!accFile)
    {
        printf("Failed to open account file.\n");
        return;
    }

    char fileLine[256];
    char userPIN[10] = "";
    char userType[20] = "";

    while (fgets(fileLine, sizeof(fileLine), accFile))
    {
        if (strncmp(fileLine, "PIN:", 4) == 0)
        {
            strcpy(userPIN, fileLine + 5);
            trimWhitespace(userPIN);
        }
        else if (strncmp(fileLine, "Account Type:", 12) == 0)
        {
            strcpy(userType, fileLine + 13);
            trimWhitespace(userType);
        }
    }
    fclose(accFile);

    char inputPIN[10];
    while (1)
    {
        printf("Enter your 4-digit PIN: ");

        if (fgets(inputPIN, sizeof(inputPIN), stdin) == NULL)
        {
            printf("Input error.\n\n");
            continue;
        }

        trimWhitespace(inputPIN);

        if (strlen(inputPIN) != 4 || !isNumericString(inputPIN))
        {
            printf("PIN must be exactly 4 digits.\n\n");
            continue;
        }

        if (strcmp(inputPIN, userPIN) != 0)
        {
            printf("Incorrect PIN. Try again.\n\n");
            continue;
        }

        break;
    }

    printf("PIN verified successfully.\n\n");

    // Account Balance Section
    accFile = fopen(userPath, "r");
    if (!accFile)
    {
        printf("Failed to read account.\n");
        return;
    }

    double userBalance = 0;
    while (fgets(fileLine, sizeof(fileLine), accFile))
    {
        if (strncmp(fileLine, "Balance:", 8) == 0)
        {
            userBalance = atof(fileLine + 8);
            break;
        }
    }
    fclose(accFile);

    printf("Your Account Number: %ld\n", userAcc);
    printf("Account Type: %s\n", userType);
    printf("Current Balance: RM %.2f\n\n", userBalance);

    // Recipient Section
    long recAcc;
    char recPath[256];
    char recType[20] = "";

    while (1)
    {
        printf("Enter recipient's account number: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error.\n\n");
            continue;
        }

        trimWhitespace(input);

        if (!isNumericString(input))
        {
            printf("Account number must contain digits only.\n\n");
            continue;
        }

        recAcc = atol(input);
        if (recAcc <= 0)
        {
            printf("Invalid account number.\n\n");
            continue;
        }

        sprintf(recPath, "database/%ld.txt", recAcc);

        FILE *recv = fopen(recPath, "r");
        if (!recv)
        {
            printf("Recipient account does not exist.\n\n");
            continue;
        }

        recType[0] = '\0'; // This is used to reset recType before reading

        // Read recipient account type
        while (fgets(fileLine, sizeof(fileLine), recv))
        {
            if (strncmp(fileLine, "Account Type:", 12) == 0)
            {
                strcpy(recType, fileLine + 13);
                trimWhitespace(recType);
                break;
            }
        }

        fclose(recv);
        // If account type is missing or empty
        if (strlen(recType) == 0)
        {
            printf("ERROR: Recipient account missing AccountType field.\n\n");
            continue;
        }

        break;
        break;
    }

    printf("Recipient Account %ld found.\n", recAcc);
    printf("Recipient Account Type: %s\n\n", recType);

    // Amount to Send Section
    int sendAmount;

    while (1)
    {
        printf("Enter amount to send (RM 1 - RM 50000): ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Input error.\n\n");
            continue;
        }

        trimWhitespace(input);

        if (!isNumericString(input))
        {
            printf("Amount must contain digits only.\n\n");
            continue;
        }

        sendAmount = atoi(input);

        if (sendAmount < 1 || sendAmount > 50000)
        {
            printf("Amount must be between RM 1 and RM 50000.\n\n");
            continue;
        }

        break;
    }

    // Calculation of Fees Section
    double fee = 0.0;

    if (strcmp(userType, "Savings") == 0 && strcmp(recType, "Current") == 0)
    {
        fee = sendAmount * 0.02; // 2%
    }
    else if (strcmp(userType, "Current") == 0 && strcmp(recType, "Savings") == 0)
    {
        fee = sendAmount * 0.03; // 3%
    }

    double totalDeduct = sendAmount + fee;

    if (totalDeduct > userBalance)
    {
        printf("Insufficient balance to cover transfer and fee.\n");
        printf("Required: RM %.2f, Available: RM %.2f\n\n", totalDeduct, userBalance);
        return;
    }

    printf("Transfer Amount: RM %d\n", sendAmount);
    printf("Remittance Fee: RM %.2f\n", fee);
    printf("Total Deduction: RM %.2f\n\n", totalDeduct);

    // Confirmation Section
    char confirm[10];

    while (1)
    {
        printf("Confirm transfer of RM %d to %ld? (yes/no): ",
               sendAmount, recAcc);

        if (fgets(confirm, sizeof(confirm), stdin) == NULL)
        {
            printf("Input error. Try again.\n\n");
            continue;
        }

        trimWhitespace(confirm);
        toLowerCase(confirm);

        if (strcmp(confirm, "yes") == 0)
            break;

        if (strcmp(confirm, "no") == 0)
        {
            printf("\nTransfer cancelled.\n\n");
            return;
        }

        printf("Invalid input. Enter 'yes' or 'no'.\n\n");
    }

    double newuserBalance = userBalance - totalDeduct;

    // Update Current User Balance
    char userTemp[256];
    sprintf(userTemp, "database/%ld_temp.txt", userAcc);

    FILE *orig = fopen(userPath, "r");
    FILE *temp = fopen(userTemp, "w");

    while (fgets(fileLine, sizeof(fileLine), orig))
    {
        if (strncmp(fileLine, "Balance:", 8) == 0)
            fprintf(temp, "Balance: %.2f\n", newuserBalance);
        else
            fputs(fileLine, temp);
    }

    fclose(orig);
    fclose(temp);
    remove(userPath);
    rename(userTemp, userPath);

    // Update Receipient Balance
    double recBalance = 0;

    FILE *recvF = fopen(recPath, "r");
    while (fgets(fileLine, sizeof(fileLine), recvF))
    {
        if (strncmp(fileLine, "Balance:", 8) == 0)
        {
            recBalance = atof(fileLine + 8);
            break;
        }
    }
    fclose(recvF);

    double newrecBalance = recBalance + sendAmount;

    char recvTemp[256];
    sprintf(recvTemp, "database/%ld_temp.txt", recAcc);

    orig = fopen(recPath, "r");
    temp = fopen(recvTemp, "w");

    while (fgets(fileLine, sizeof(fileLine), orig))
    {
        if (strncmp(fileLine, "Balance:", 8) == 0)
            fprintf(temp, "Balance: %.2f\n", newrecBalance);
        else
            fputs(fileLine, temp);
    }

    fclose(orig);
    fclose(temp);
    remove(recPath);
    rename(recvTemp, recPath);

    // Receipt Section
    printf("\n======================================\n\n");
    printf("        REMITTANCE RECEIPT\n\n");
    printf("======================================\n\n");
    printf("User Account: %ld (%s)\n", userAcc, userType);
    printf("Recipient Account: %ld (%s)\n", recAcc, recType);
    printf("Amount Sent: RM %d\n", sendAmount);
    printf("Remittance Fee: RM %.2f\n", fee);
    printf("Total Deducted: RM %.2f\n", totalDeduct);
    printf("New User Balance: RM %.2f\n", newuserBalance);
    printf("=========================================\n\n");
}