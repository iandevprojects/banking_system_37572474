#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utilities.h"

// This function is used to count the number of accounts in the database
int countAccounts() {
    FILE *indexFile;
    char line[50];
    int count = 0;

    indexFile = fopen("database/index.txt", "r");
    if (!indexFile) return 0;

    while (fgets(line, sizeof(line), indexFile)) count++;

    fclose(indexFile);
    return count;
}

// This function is used to trim any whitespace which exists
void trimWhitespace(char *str) {
    char *start = str;
    while (isspace((unsigned char)* start)) start++;

    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    // This was added to move the string to the start
    memmove(str, start, end - start + 2);
}

// This function is used to check if the string is fully alphabetical
int isAlphabeticString(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

// This function is used to check if the string is fully digits
int isNumericString(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}

// This function is created to convert a string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}