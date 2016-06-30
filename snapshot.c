/* Alexander Simchuk, snapshot.c, Version 2.0
   This is a curses application to allow unix users to access their zfs snapshots and copy the
   contents to their homedir.
   */
#include "snapshot.h"
#include "path_interpreter.h"
#include "print_messages.h"

//int fileInDir(char * path,char * dir,);

int main(int argc, char * argv[]) {

    if(argc != 2) {
        printUsageMessageThenExit();
    }

    if( strcmp(argv[1],"--help") == 0 || strcmp(argv[1],"-h") == 0 ) {
        printHelpMessage();
        exit(-1);
    }

    FULLPATH current_path;

    current_path.input_file = genSearchPath(argv[1]);

    int cntr, num_items, choice;

    FILE *fp;
    char buffer[50], **choices_references, **choices_main;

    //initialize char array for main menu options
    if(!(fp = popen("/usr/local/lib/list_snapshots.sh","r"))) {
        perror("Cannot access list of dates\n");
        exit(-1);
    }

    cntr = 0;
    while ((fgets(buffer, 50,fp)) != NULL) {
        cntr++;
    }
    pclose(fp);

    num_items = (cntr - 2) / 2;

    EXIT_IF_NULL( (choices_main = (char **)calloc(num_items+1,sizeof(char *))) ,
            "ERROR:main(): could not allocate memory for char* 1");
    EXIT_IF_NULL( (choices_references = (char **)calloc(num_items+1,sizeof(char *))) ,
            "ERROR:main(): could not allocate memory for char* 1");

    if(!(fp = popen("/usr/local/lib/list_snapshots.sh","r"))) {
        perror("Cannot access list of dates\n");
        exit(-1);
    }

    //set the beginning of the absolute path for the snapshots
    if((fgets(buffer, 50,fp)) != NULL) {
        current_path.base = strdup(buffer);
        if(current_path.base[(strlen(current_path.base)-1)] == '\n') {
            current_path.base[(strlen(current_path.base)-1)] = '\0';
        }
    }

    cntr = 0;

    char path_buffer[PATH_MAX];
    struct stat temp;
    int file_exists;

    while (fgets(buffer, 50,fp) != NULL) {

        choices_references[cntr] = strdup(buffer);

        if (choices_references[cntr][(strlen(choices_references[cntr])-1)] == '\n') {
            choices_references[cntr][(strlen(choices_references[cntr])-1)] = '\0';
        }

        fgets(buffer, 50,fp);
        choices_main[cntr] = strdup(buffer);

        if (choices_main[cntr][(strlen(choices_main[cntr])-1)] == '\n') {
            choices_main[cntr][(strlen(choices_main[cntr])-1)] = '\0';
        }

        memset(path_buffer,0,PATH_MAX*sizeof(char));
        errno = 0;
        snprintf(path_buffer,PATH_MAX*sizeof(char),"%s/%s%s",current_path.base,choices_references[cntr],current_path.input_file);
        if( errno ) { perror("snprintf"); }

        //printf("Checking: %s ...",path_buffer);

        file_exists = 1;
        file_exists = stat(path_buffer,&temp);

        if( cntr > 0 && strcmp(choices_main[cntr],choices_main[cntr-1]) == 0 ) {
            file_exists = 1;
        }

        if( file_exists == 0 ) {
            cntr++;
            //printf(" !\n");
        }
        else {
            //printf(" ...\n");
            free(choices_references[cntr]);
            free(choices_main[cntr]);
        }
    }
    pclose(fp);

    if( cntr == 0) {
        printf("\n\"/home%s\" does not exist in any of the available backups.\nIf the file was created within the last hour or more than fifteen"
                " days ago, it may no longer exists in our backups. Otherwise, check your spelling and try again.\n",current_path.input_file);
        exit(-1);
    }

    num_items = cntr;

    choice = mainMenu("Available versions",choices_main,num_items);

    system("clear");
    printf("choice is %d\n", choice);

    if(choice >= 0) {

        printInstructions();

        memset(path_buffer,0,PATH_MAX*sizeof(char));
        errno = 0;
        snprintf(path_buffer,PATH_MAX*sizeof(char),"cp -r %s/%s%s ~/backup_recovery/",current_path.base,choices_references[choice],current_path.input_file);
        if( errno ) { perror("snprintf"); }

        if ( (system("mkdir -p ~/backup_recovery")) == -1) {
            printf("Failed to make directory ~/backup_recovery");
            exit(-1);
        }
        if ( (system(path_buffer)) == -1) {
            printf("Failed to cp file into ~/backup_recovery");
            exit(-1);
        }
    }

    free(choices_references);
    free(current_path.input_file);

    return 0;
}

