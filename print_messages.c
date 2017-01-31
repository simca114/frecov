/* Alexander Simchuk, print_messages.c
 *
 * This file contains the definitions of the methods
 * that are used to print messages for the users
 */

#include "print_messages.h"

// TODO: write comments for the functions purpose
void printHelpMessage() {

    char buffer[500];

    // TODO: call the getCurrentUser() function from sustem_calls.c
    char *user = getenv("USER");

    FILE * fp;

    if(!(fp = fopen("/usr/local/lib/frecov_forms/help_message.txt","r"))) {
        perror("Cannot access -h file\n");
        exit(-1);
    }

    while ((fgets(buffer, 500,fp)) != NULL) {
        if(strstr(buffer,"EXAMPLE") != NULL) {
            printf(buffer,user);
        }
        else {
            printf("%s",buffer);
        }
    }

    fclose(fp);
}

// TODO: write comments for the functions purpose
void printInstructions() {

    char buffer[500];

    char *user = getenv("USER");

    //FILE * fp = fopen("/usr/local/lib/frecov_forms/instructions_message.txt", "r");
    FILE* dummy = malloc(sizeof(FILE));
    FILE *filep = fdopen(open("/usr/local/lib/frecov_forms/instructions_message.txt",O_RDONLY),"r");

    if(!filep) {
        perror("Cannot access -h file\n");
        exit(-1);
    }

    while ((fgets(buffer, 500,filep)) != NULL) {
        printf("%s",buffer);
    }

    fclose(filep);
}

// TODO: write comments for the functions purpose
void printNoBackupFound() {

    char buffer[500];

    // TODO: call the getCurrentUser() function from sustem_calls.c
    char *user = getenv("USER");

    FILE * fp;

    if(!(fp = fopen("/usr/local/lib/frecov_forms/no_backup_message.txt","r"))) {
        perror("Cannot access no backup message file\n");
        exit(-1);
    }

    while ((fgets(buffer, 500,fp)) != NULL) {
        if(strstr(buffer,"EXAMPLE") != NULL) {
            printf(buffer,user);
        }
        else {
            printf("%s",buffer);
        }
    }

    fclose(fp);

}

// TODO: write comments for the functions purpose
void printUsageMessageThenExit() {
    printf("Usage: frecov /path/to/$FILE\n");
    printf("Use -h | --help for detailed usage instructions\n");
    exit(-1);
}

// TODO: write comments for the functions purpose
void printPathExampleThenExit(char *user) {
    printf("Please enter a valid absolute path (ex. /home/%s/ubuntu/$pathToFile or /u/%s/$pathToFile)\n",user,user);
    exit(-1);
}
