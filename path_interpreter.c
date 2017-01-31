/* Simchuk Alexander, path_interpreter.c
 *
 * This file is for the implementation of the path interpreter used
 * in frecov, 
 */ 

#include "system_info.h"

//TODO: look over document and revise function order if necessary

/* char *getBasePath()
 *
 * This function finds the path prefix of where the snapshots are stored for the 
 * current user.
 *
 * MEMORY: returned string is dynamically allocated, will need to be freed by caller
 *         when finished with it
 *
 * INPUT: NA
 *
 * RETURNS: char* of the absolute path of where the users snapshots are.
 */
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

/* char *getSearchPath(char *user_input)
 *
 * This function parses the users inputted path and generates a search string that
 * can be appended to the snapshot path prefix
 *
 * MEMORY: returned string is dynamically allocated, will need to be freed by caller
 *         when finished with it
 *
 * INPUT: char* of the user's inputted path
 *
 * RETURNS: char* of the path to the file from the users homedir base.
 */
//TODO: generate tests for relative path inputs
char *genSearchPath(char *user_input) {
    char *new_string, *buffer, *string_parts[21];
    int counter = 0; 
    char *user = NULL;
    char *distro = NULL;
    char **path_tokens = NULL;
    char *user_input_copy = strdup(user_input);

    char* path_type = getPathType(user_input_copy);
    if (!path_type) {
        printPathExampleThenExit(user);
    }

    user = getCurrentUser();
    distro = getCurrentDistro();

    path_tokens = splitPath(user_input_copy);

    if (strcmp(path_type,"abs_hom") == 0) {
        // check the abs_home path prefix
        if (validAbsHome(path_tokens[0], path_tokens[1], path_tokens[2]) == false) {
            printPathExampleThenExit(user);
        }
        distro = path_tokens[2];
        path_tokens = removeBegArray(path_tokens, 3);
    } else if (strcmp(path_type, "abs_cat") == 0) {
        if (validAbsCat(path_tokens[0], path_tokens[1]) == false) {
            printPathExampleThenExit(user);
        }
        path_tokens = removeBegArray(path_tokens, 2);
    } else if (strcmp(path_type, "rel_hom") == 0) {
        path_tokens = removeBegArray(path_tokens, 1);
    } else {
        path_tokens = relativePathTokens(path_tokens);
    }

    char *path_prefix = NULL;
    char *path_suffix = NULL;
    char *path_ret = NULL;

    char *prefix_parts[] = { user, distro, NULL};
    path_prefix = concatPath(prefix_parts);

    path_suffix = concatPath(path_tokens);
    printf("path_suffix: %s\n", path_suffix);

    path_ret = (char*)malloc((strlen(path_prefix)+strlen(path_suffix)+1)*sizeof(char));
    path_ret = strcpy(path_ret, path_prefix);
    path_ret = strcat(path_ret, path_suffix);

    free(user_input_copy);

    return path_ret;
}

/* char **relativePathTokens(char **current_tokens)
 *
 * This function finds the missing path components from the users homedir base to the
 * inputted file. This is necessary if the user provides a relative path.
 *
 * MEMORY: TODO check if memory needs to be freed.
 *
 * INPUT: char** of the path tokens already provided by the user
 *
 * RETURNS: char** of the path tokens from the base of the homedir to the inputted file.
 */
//TODO: generate tests for this function
char **relativePathTokens(char **current_tokens) {
    char **new_path_tokens = NULL;
    char *full_path = getenv("PWD");

    printf("path: %s\n", full_path);

    // tokenize the path and remove the prefixed /u/$USER
    char **full_path_tokens = splitPath(full_path);
    full_path_tokens = removeBegArray(full_path_tokens, 2);

    int depth_from_home = stringArrayLen(full_path_tokens);

    // get the number of "../"'s in the input path
    int up_dirs = 0;
    while(strcmp(current_tokens[up_dirs], "..") == 0) {
        up_dirs++;
    }

    if (up_dirs > 0) {
        //make sure we aren't going outside of the homedir
        if (up_dirs > depth_from_home) {
            perror("ERROR: relativePathTokens(): relative path given goes outside of home directory");
            return NULL;
        }

        current_tokens = removeBegArray(current_tokens, up_dirs);
        while (up_dirs > 0) {
            // this may need to be freed before being set to null
            //free(full_path_tokens[depth_from_home-1]);
            full_path_tokens[depth_from_home-1] = NULL;

            depth_from_home--;
            up_dirs--;
        }

    }

    // build the new token list
    int new_len = stringArrayLen(full_path_tokens) + stringArrayLen(current_tokens);
    new_path_tokens = (char**)malloc((new_len+1)*sizeof(char*));
    memset(new_path_tokens, '\0', (new_len+1)*sizeof(char*));

    int counter, position = 0;;
    for (counter = 0; counter < stringArrayLen(full_path_tokens); counter++) {
        new_path_tokens[position] = full_path_tokens[counter];
        position++;
    }

    for (counter = 0; counter < stringArrayLen(current_tokens); counter++) {
        new_path_tokens[position] = current_tokens[counter];
        position++;
    }

    for (counter = 0; counter < stringArrayLen(new_path_tokens); counter++) {
        printf("%d: %s\n", counter, new_path_tokens[counter]);
    }

    free(full_path_tokens);

    return new_path_tokens;
}

/* char *getPathType(char *path)
 *
 * This function takes the users inputted path and determines what type of path
 * was provided. (NOTE: the names for the path types were invented for this program
 * and are likely to not be referenced anywhere else)
 *
 * MEMORY: returned string is dynamically allocated, will need to be freed by caller
 *         when finished with it
 *
 * INPUT: char* of the users inputted path.
 *
 * RETURNS: char* of the type of path the inputted path is.
 */
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
    else if (path[0] == '~') { strcpy(type, "rel_hom");
    }
    else {
        strcpy(type, "rel_cwd");
    }

    return type;
}

/* char *getAbsolutePathBase(char *path)
 *
 * This function generates an absolute path for the snapshot file to be copied to
 *
 * MEMORY: returned string is dynamically allocated, will need to be freed by caller
 *         when finished with it
 *
 * INPUT: char* of the absolute path to the user inputted file
 *
 * RETURNS: char* containing the path the snapshot file should be copied to.
 */
//TODO: generate tests for this function
//TODO: more descriptive return messages for error returns
//TODO: rename this function to something that reflects it being the copy destination path
char *getAbsolutePathBase(char *path) {

    if (!path || strlen(path) == 0 || path[0] != '/') {
        return NULL;
    }

    char *abs_path = NULL;
    char *part1, *part2, *part3;

    part1 = strtok(path, "/\n ");
    part2 = strtok(0, "/\n ");
    part3 = strtok(0, "/\n ");

    if (validAbsHome(part1, part2, part3) == false) {
        return NULL;
    }

    // allocate new path space
    // length of each part
    // + 3 /'s and 1 terminating null char
    int len = 20 + strlen(part1) + strlen(part2) + strlen(part3);
    abs_path = (char*)malloc((len+1)*sizeof(char));
    memset(abs_path, '\0', (len+1)*sizeof(char));

    //TODO: error check this call
    snprintf(abs_path, (len)*sizeof(char), "/%s/%s/%s/backup_recovery", part1, part2, part3);

    return abs_path;
}

//TODO: generate tests for this function
//TODO: more descriptive return messages for error returns
//TODO: write comments for the functions purpose
char *getRelativePathBase() {
    char *rel_path = (char*)malloc(18*sizeof(char)); strcpy(rel_path, "~/backup_recovery\0");

    return rel_path;
}

/* bool validAbsHome(char *home, char *user, char *distro)
 *
 * This function checks whether the inputted args are valid starting tokens for a
 * absolute path.
 *
 * INPUT: char* home: the first token of a absolute path (should be "home")
 *        char* user: the second token of a absolute path (should be the logged in user ONLY)
 *        char* distro: the third token of a absolute path (should be a valid distro that exists)
 *
 * RETURNS: True if all tests pass and the three tokens are valid, false otherwise.
 */
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

/* bool validAbsCat(char *u, char *user)
 *
 * This function checks whether the inputted args are valid starting tokens for a absolute
 * CAT path.
 *
 * INPUT: char* u: the first token of a absolute CAT path (should be "u")
 *        char* user: the second token of a absoluteCAT path (should be the logged in user ONLY)
 *
 * RETURNS: True if all tests pass and the two tokens are valid, false otherwise.
 */
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

// TODO: create tests for this function
//TODO: write comments for the functions purpose
char *concatFULLPATH(FULLPATH input) {
    int full_len = strlen(input.base) + strlen(input.timestamp) + strlen(input.input_file);
    char *file_to_check = (char*)malloc((full_len+1)*sizeof(char));
    memset(file_to_check, '\0', (full_len+1)*sizeof(char));

    strncpy(file_to_check, input.base, strlen(input.base));
    strncat(file_to_check, input.timestamp, strlen(input.timestamp));
    strncat(file_to_check, input.input_file, strlen(input.input_file));

    return file_to_check;
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

//TODO: write comments for the functions purpose
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
