/* Alexander Simchuk, print_messages.c
 *
 * This file contains the definitions of the methods
 * that are used to print messages for the users
 */

#include "snapshot.h"

void printHelpMessage() {

    char buffer[500];

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
