#include <stdio.h>
#include <string.h>
#include "credentials.h"

/*
Files would be setup like 

USERNAME, PASSWORD

which allows us to use a CSV to store credentials

*/


void read_credentials(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    char line[256];
    if (fgets(line, sizeof(line), file)) {
        char *comma_pos = strchr(line, ',');
        *comma_pos = '\0';
        char *username = line;
        char *password = comma_pos + 1;
        while (*password == ' ') password++;        
    }

    fclose(file);
}

void add_credentials(const char *filename, const char *username, const char *password) {
    FILE *file = fopen(filename, "a");  // Open in append mode
    if (file == NULL) {
        return;  // Error opening file
    }

    // Write username and password to the file
    fprintf(file, "%s, %s\n", username, password);

    fclose(file);
}

