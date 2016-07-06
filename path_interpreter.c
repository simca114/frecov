/* Simchuk Alexander, path_interpreter.c
 *
 * This file is for the implementation of the path interpreter used
 * in frecov, */ 

#include "path_interpreter.h"
#include "string_manip.h"
#include "print_messages.h"
#include "system_info.h"
#include "validation_macros.h"

//TODO: look over document and revise function order if necessary
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
            perror("ERROR:sprintf"); exit(-1); }

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

char *getPathType(char *path) {
    // will need to free returned char*
    char *type = (char*)calloc(8, sizeof(char));

    if (!path) {
        free(type);
        type = NULL;
    }
    else if (strlen(path) == 0) {
        free(type);
        type = NULL;
    }
    else if (path[0] == '/') {
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

bool validAbsHome(char *home, char *user, char *distro) {

    //if any of the following tests fail we want to stop evaluating and return false
    if (!home || !user || !distro) {
        return false;
    }
    else if (strcmp("home", home) != 0) {
        return false;
    }
    else if (strcmp(getCurrentUser(), user) != 0) {
        return false;
    }

    // dont bother testing further if either of the first two tests failed
    if (strcmp("ubuntu", distro) == 0) {}
    else if (strcmp("redhat5", distro) == 0) {}
    else if (strcmp("redhat6", distro) == 0) {}
    else if (strcmp("solaris", distro) == 0) {}
    else if (strcmp("common", distro) == 0) {}
    else if (strcmp("osx", distro) == 0) {}
    else if (strcmp("mail", distro) == 0) {}
    else {
        return false;
    }

    return true;
}

bool validAbsCat(char *u, char *user) {

    if (!u || !user) {
        return false;
    }
    else if (strcmp("u", u) != 0) {
        return false;
    }
    else if (strcmp(getCurrentUser(), user) != 0) {
        return false;
    }

    return true;
}

/* char *concatPath(char **ordered_path)
 *
 * This function will create a path string from the given string components.
 * Each component will be prepended with a forward slash (/).
 *
 * MEMORY: returned string is dynamically allocated, will need to be freed by caller
 *         when finished with it
 *
 * INPUT: char** of the path components to be concatinated
 *
 * RETURNS: char* of the generated path
 */
char *concatPath(char **ordered_path) {

    if (!ordered_path) {
        return NULL;
    }

    int num_items = stringArrayLen(ordered_path);

    if (num_items == 0) {
        return NULL;
    }

    //we need to calculate how long the resulting char* will be.
    //first we count the number of /'s that will be generated and the terminating '\0'
    int new_len = num_items+1;
    //now we add the lengths of each of the path components
    int ctr = 0;
    for (ctr = 0; ctr < num_items; ctr++) {
        new_len += strlen(ordered_path[ctr]);
    }

    char *ret_path = (char*)malloc(new_len*sizeof(char));
    memset(ret_path, 0, new_len*sizeof(char));

    for (ctr = 0; ctr < num_items; ctr++) {
        strncat(ret_path, "/", 1);
        strncat(ret_path, ordered_path[ctr], strlen(ordered_path[ctr]));
    }

    return ret_path;
}

char **splitPath(char *path) {
    int num_tokens = 0;

    if (!path) {
        return NULL;
    }
    if (strlen(path) == 0) {
        return NULL;
    }
    if ((strlen(path) == 1) && path[0] == '/') {
        return NULL;
    }

    //offset the prefix '/' if it exists
    if (path[0] == '/') {
        num_tokens--;
    }
    //check if the array ends with a '/' and assign the proper offset
    if (path[strlen(path)-1] == '/') {
        num_tokens--;
    }

    //get the number of path components in the inputted path
    int ctr = 0;
    for (ctr = 0; ctr < strlen(path); ctr++) {
        if (path[ctr] == '/') {
            num_tokens++;
        }
    }
    // add one more for the lea
    num_tokens++;

    char **split_path = (char**)malloc((num_tokens+1)*sizeof(char*));
    EXIT_IF_NULL(split_path, "ERROR splitPath(path): Could not allocate memory for split_path");

    split_path[0] = strtok(path, "/\n");

    ctr = 1;
    while ((split_path[ctr] = strtok(0, "/\n"))) {
        ctr++;
    }

    //display output as test
    /*
    for (ctr = 0; ctr < num_tokens+1;ctr++) {
        printf("ctr %d: value is %s\n", ctr, split_path[ctr]);
    }
    */

    return split_path;
}
