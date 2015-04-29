/* Alexander Simchuk, snapshot.c, Version 2.0
   This is a curses application to allow unix users to access their zfs snapshots and copy the
   contents to their homedir.
   */
#include "snapshot.h"

//int fileInDir(char * path,char * dir,);

int main(int argc, char * argv[]) {

    if(argc != 2) {
        printf("Usage: frecov /path/to/$FILE\n");
	printf("Use -h | --help for detailed usage instructions\n");
        exit(-1);
    }

    if( strcmp(argv[1],"--help") == 0 || strcmp(argv[1],"-h") == 0 ) {
        printHelpMessage();
        exit(-1);
    }

    FULLPATH current_path;

    current_path.input_file = interpretPath(argv[1]);

    int cntr, num_items, choice;

    FILE *fp;
    char buffer[50], **choices_references, **choices_main;

    //initialize char array for main menu options
    if(!(fp = popen("/usr/bin/list_dates.sh","r"))) {
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

    if(!(fp = popen("/usr/bin/list_dates.sh","r"))) {
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

    return 0;
}

void printHelpMessage() {

    char *user = getenv("USER");

    printf("Usage: frecov FILEPATH\n");
    printf("Scans available snapshots for FILEPATH. FILEPATH can point to either a file name or a directory name.\n");
    printf("If at least one version is found, a menu will display prompting you for a choice.\n");
    printf("If no version of FILEPATH is found, it does not exist in our snapshots.\n");
    printf("Possible reasons for this could be because FILEPATH was created within the last hour or FILEPATH has not existed in your home directory for more than 15 days.\n");
    printf("\n");
    printf("FILEPATH is a string with the following formats:\n");
    printf("    - Absolute path using default path ( /home/$user/$homedir/ )\n");
    printf("    EXAMPLE: /home/%s/ubuntu/somedir/path_to_file\n",user);
    printf("\n");
    printf("    - Absolute path using /u symlink (points to homedir of operating system the computer you are using is loaded with) \n");
    printf("    EXAMPLE: /u/%s/path_to_file\n",user);
    printf("\n");
    printf("    - Relative path from homedir (points to homedir of operating system the computer you are using is loaded with)\n");
    printf("    EXAMPLE: ~/path_to_file\n");
    printf("\n");
    printf("Report bugs to the Computer Action Team: support@cat.pdx.edu\n");
    printf("Use \'frecov bug\' as the subject and provide as much details as you can regarding the nature of the bug in the body of the message\n");

}

void printInstructions() {

    printf("The file and version you have chosen have been copied into a directory called\n");
    printf("\"backup_recovery\" which has been placed in your home directory.\n");
    printf("\n");
    printf("Contact the Computer Action Team if you have any questions: \n");
    printf("email support@cat.pdx.edu or call 503-725-5420 \n");

}

char * interpretPath(char *user_input) {

    char *new_string, *user, distrobuffer[100], *buffer, *string_parts[21];
    int counter = 0;

    EXIT_IF_NULL( (user = getenv("USER")) ,
            "ERROR: interpretPath(): getenv user failed\n");

    if( (user_input[0] == '~' && user_input[1] == '/') || (user_input[0] == '/' && user_input[1] == 'u') ) {
        new_string = malloc( ((8 + strlen(user))+1) * sizeof(char) );

        FILE *file;

        file = fopen("/etc/auto.master","r");

        if(!file) {
            file = fopen("/etc/auto_master","r");
            EXIT_IF_NULL( file, "ERROR:interpretPath(): failed to open file\n");
        }

        while((fgets(distrobuffer,100,file))) {
            if(distrobuffer[1] == 'u') {
                buffer = strtok(distrobuffer,"_,");
                buffer = strtok(0,"_,");
                buffer = strtok(0,"_,");

                if(strcmp(buffer,"linux") == 0) {
                    buffer = "ubuntu";
                }
                break;
            }
        }
        fclose(file);

        EXIT_IF_NULL( buffer , "ERROR:interpretPath(): failed to get distro");

        string_parts[0] = malloc((strlen(user)+strlen(buffer)+3)*sizeof(char));
        errno = 0;
        sprintf(string_parts[0],"/%s/%s",user,buffer);

        if(errno != 0) {
            perror("ERROR:sprintf");
            exit(-1);
        }

        //offset the strtok buffer
        buffer = strtok(user_input,"/\n");

        if( strcmp(buffer,"u") == 0 ) {
            buffer = strtok(0,"/\n");
            if( (strcmp(buffer,user)) ) {
                printPathExampleThenExit();
            }
        }
        else {
            buffer = strtok(0,"/\n");
        }
    }
    else if (user_input[0] != '/' && user_input[0] != '~') {
        printPathExampleThenExit(user);
    }
    else {
        buffer = strtok(user_input, "/\n");

        if( (strcmp(buffer,"home")) ) {
            printPathExampleThenExit(user);
        }

        for(counter = 1; counter <= 2; counter++) {
            if( !(buffer = strtok(0, "/\n")) ) {
                printPathExampleThenExit();
            }
        }

        if( (strcmp(buffer,"common")) && (strcmp(buffer,"mail")) && (strcmp(buffer,"osx")) && (strcmp(buffer,"redhat5")) &&
                (strcmp(buffer,"redhat6")) && (strcmp(buffer,"solaris")) && (strcmp(buffer,"ubuntu")) ) {
            printPathExampleThenExit(user);
        }

        new_string = malloc( ((2 + strlen(user) + strlen(buffer))+1) * sizeof(char));
        errno = 0;
        sprintf(string_parts[0],"/%s/%s",user,buffer);

        if(errno != 0) {
            perror("ERROR:sprintf");
            exit(-1);
        }
    }


    counter = 1;
    while( (buffer = strtok(0, "/\n")) ) {
        string_parts[counter] = strdup(buffer);
        counter++;
    }

    //if the program didnt enter the while loop, only base path was provided, no file
    if( counter == 1 ) {
        printPathExampleThenExit(user);
    }

    int max_tokens = counter;
    int path_length = counter;
    for(counter = 0; counter < max_tokens;counter++) {
        path_length += strlen(string_parts[counter]);
    }

    new_string = malloc((path_length+1) * sizeof(char));
    new_string[0] = 0;
    for(counter = 0; counter < max_tokens; counter++) {
        new_string = strcat(new_string,string_parts[counter]);
        if(counter != max_tokens-1) {
            new_string = strcat(new_string,"/");
        }
    }

    for(counter = 0; counter < max_tokens; counter++) {
        free(string_parts[counter]);
    }

    return new_string;
}

void printPathExampleThenExit() {
    printf("Usage: frecov FILEPATH\n");
    printf("Use -h | --help for detailed usage instructions\n");
    exit(-1);
}
