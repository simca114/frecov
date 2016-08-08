/* Simchuk Alexander, path_interpreter.c
 *
 * This file is for the implementation of the path interpreter used
 * in frecov, */ 

#include "system_info.h"


//TODO: look over document and revise function order if necessary

char *getBasePath() {
    char buffer[100], *base_path;
    FILE *fp;

    if(!(fp = popen("/usr/local/lib/get_snapshot_path.sh","r"))) {
        perror("Cannot open base path info\n");
        exit(-1);
    }

    if((fgets(buffer, 100,fp)) != NULL) {
        base_path = strdup(buffer);
        base_path = stripNewline(base_path);
    } else {
        perror("Cannot read base path from file\n");
        exit(-1);
    }

    if((pclose(fp)) == -1) {
        perror("cant close file stream...");
        exit(-1);
    }

    return base_path;
}

char *genSearchPath(char *user_input) {
    char *new_string, *buffer, *string_parts[21];
    int counter = 0;

    char *user = NULL;
    char *distro = NULL;
    char **path_tokens = NULL;

    //TODO:
    printf("user_input: %s\n", user_input);
    char* path_type = getPathType(user_input);
    if (!path_type) {
        printPathExampleThenExit(user);
    }
    printf("path_type: %s\n", path_type);

    user = getCurrentUser();
    distro = getCurrentDistro();

    path_tokens = splitPath(user_input);

    if (strcmp(path_type,"abs_hom") == 0) {
        printf("is abs_home\n");
        // check the abs_home path prefix
        if (validAbsHome(path_tokens[0], path_tokens[1], path_tokens[2]) == false) {
            printPathExampleThenExit(user);
        }
        distro = path_tokens[2];
        path_tokens = removeBegArray(path_tokens, 3);
    }
    else if (strcmp(path_type, "abs_cat") == 0) {
        if (validAbsCat(path_tokens[0], path_tokens[1]) == false) {
            printPathExampleThenExit(user);
        }
        path_tokens = removeBegArray(path_tokens, 2);
    }
    else if (strcmp(path_type, "rel_hom") == 0) {
        path_tokens = removeBegArray(path_tokens, 1);
    }

    char *path_prefix = NULL;
    char *path_suffix = NULL;
    char *path_ret = NULL;

    char *prefix_parts[] = { user, distro, NULL};
    path_prefix = concatPath(prefix_parts);
    printf("path_prefix: %s\n", path_prefix);

    path_suffix = concatPath(path_tokens);
    printf("path_suffix: %s\n", path_suffix);

    path_ret = (char*)malloc((strlen(path_prefix)+strlen(path_suffix)+1)*sizeof(char));
    path_ret = strcpy(path_ret, path_prefix);
    path_ret = strcat(path_ret, path_suffix);

    return path_ret;
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

    return split_path;
}
