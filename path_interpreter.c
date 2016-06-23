/* Simchuk Alexander, path_interpreter.c
 *
 * This file is for the implementation of the path interpreter used
 * in frecov,
 */

#include "snapshot.h"
char *genSearchPath(char *user_input) { /*
    char *new_string, *buffer, *string_parts[21];
    int counter = 0;

    char *user = NULL;
    char *distro = NULL;
    char **path_tokens = NULL;

    char* path_type = getPathType(user_input);

    user = getCurrentUser();
    distro = getCurrentDistro();

    path_tokens = splitPath(user_input);

    if (strcmp(path_type,"abs_home") == 0) {
        // check the abs_home path prefix
        if (validAbsHome(path_tokens[0], path_tokens[1], path_tokens[2]) == false) {
            printPathExampleThenExit(user);
        }
        path_tokens = advanceArray(path_tokens, 3);
    }
    else {
        //TODO: implement handling of other paths
    }

    char *path_prefix = NULL;
    char *path_suffix = NULL;
    char *path_to_return = NULL;
    int path_ret_len = 0;

    path_prefix = (char **)calloc(strlen(user)+strlen(distro)+1,sizeof(char))
    sprintf(path_prefix,"/%s/%s",user,distro);
    */
    return "";

    /*
    if( (user_input[0] == '~' && user_input[1] == '/') || (user_input[0] == '/' && user_input[1] == 'u') ) {
        new_string = malloc( ((8 + strlen(user))+1) * sizeof(char) );
        errno = 0;
        sprintf(string_parts[0],"/%s/ubuntu",user);

        if(errno != 0) {
            perror("ERROR:sprintf");
            exit(-1);
        }

        //offset the strtok buffer
        buffer = strtok(user_input,"/\n");

        if( strcmp(buffer,"u") == 0 ) {
            buffer = strtok(0,"/\n");
            if( (strcmp(buffer,user)) ) {
                printPathExampleThenExit(user);
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
                printPathExampleThenExit(user);
            }
        }

        printf("buffer: %d\n",(strcmp(buffer,"ubuntu")));
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
    printf("num of /: %d\n", path_length);
    for(counter = 0; counter < max_tokens;counter++) {
        path_length += strlen(string_parts[counter]);
    }

    new_string = malloc((path_length+1) * sizeof(char));
    for(counter = 0; counter < max_tokens;counter++) {
        new_string = strcat(new_string,string_parts[counter]);
        if(counter != max_tokens-1) {
            new_string = strcat(new_string,"/");
        }
    }
    
    return new_string;
    */
}

char *getCurrentUser() {
    char *user;
    EXIT_IF_NULL((user = getenv("USER")) ,
                  "ERROR: interpretPath(): getenv user failed\n");
    return user;
}

char *getCurrentDistro() {
    char *distro = NULL;
    char *name = NULL;
    int redhat_ver = 0;
    FILE *fp;

    // get the output of the facter command
    if(!(fp = popen("facter","r"))) {
        perror("Cannot access list of dates\n");
        exit(-1);
    }

    char buffer[100];
    char *cur_token;
    // skip down to the line where the os var is listed
    while (fgets(buffer, 100, fp)) {
        cur_token = strtok(buffer, " \n");
        if (strcmp(cur_token,"os") == 0) {
            // solaris output is wierd, needs special care
            if (strlen(cur_token) > 10) {
                name = (char*)calloc(7,sizeof(char));
                strcpy(name,"solaris");
            }
            break;
        }
    }

    //skip down to name (unless solaris was detected)
    while (fgets(buffer, 100, fp) && !name) {
        cur_token = strtok(buffer, " \n");
        if (strcmp(cur_token,"id") == 0) {
            cur_token = strtok(0, " \"\n");
            name = strdup(strtok(0, " \"\n"));
            break;
        }
    }

    // if name is redhat, we need to find the version (5 or 6)
    if (strstr(name,"CentOS") || strstr(name,"RedHat")) {
        while (fgets(buffer, 100, fp)) {
            cur_token = strtok(buffer, " \n");
            if (strcmp(cur_token,"major") == 0) {
                cur_token = strtok(0, " \"\n");
                redhat_ver = strtok(0, " \"\n")[0] - '0';
                break;
            }
        }
    }

    int offset = (redhat_ver == 0) ? 1 : 2;
    distro = (char*)calloc(strlen(name)+offset,sizeof(char));

    if (offset == 1) {
        strcpy(distro, name);
    }
    else {
        sprintf(distro, "redhat%d", redhat_ver);
    }

    free(name);
    pclose(fp);

    int i = 0;
    for (i = 0; i < strlen(distro); i++) {
        distro[i] = tolower(distro[i]);
    }

    return distro;
}

char *getPathType(char *path) {
    char *type = (char*)calloc(8, sizeof(char));

    if (path[0] == '/') {
        if (path[1] == 'u') {
            strcpy(type, "abs_cat");
        }
        else {
            strcpy(type, "abs_hom");
        }
    }
    else if (path[0] == '~') {
        strcpy(type, "rel_hom");
    }
    else {
        strcpy(type, "rel_cwd");
    }

    return type;
}

char *genSearchPath(char *user_input);
char *concatPath(char **ordered_path);
char **splitPath(char *path);
bool validAbsHome(char *home, char *user, char *distro);
bool validAbsCat(char *u, char *user);


void printPathExampleThenExit(char *user) {
    printf("Please enter a valid absolute path (ex. /home/%s/ubuntu/$pathToFile or /u/%s/$pathToFile)\n",user,user);
    exit(-1);
}
