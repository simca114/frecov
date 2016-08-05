/* Alexander Simchuk, system_info.c
 *
 * 
 *
 */

#include "system_info.h"
#include "validation_macros.h"

char *getCurrentUser() {
    char *user;
    EXIT_IF_NULL((user = getenv("USER")) ,
            "ERROR: interpretPath(): getenv user failed\n");
    return user;
}


/* This function uses facter to get the information of the distro of the box the
 * user is calling the function from.
 *
 * Memory is being allocated for returned string, will need to be freed.
 *
 * INPUT: None
 *
 * RETURNS: Newly allocated char* containing the name of the distro.
 */
char *getCurrentDistro() {
    //will need to free returned char*
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
                name = (char*)malloc(8*sizeof(char));
                memset(name, '\0', 8*sizeof(char));
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
    distro = (char*)malloc((strlen(name)+offset+1)*sizeof(char));
    memset(distro, '\0', (strlen(name)+offset+1)*sizeof(char));

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

// TODO: run tests on this
SNAPINFO** searchSnapshotsForFile(char *base, char *input_file) {
    FULLPATH search_path;
    search_path.base = base;
    search_path.input_file = input_file;

    int snap_count = getTotalSnapshotCount();
    SNAPINFO **all_snapshots = getSnapshotInfo(snap_count);

    SNAPINFO **found_snapshots = (SNAPINFO**)malloc(sizeof(SNAPINFO*));
    memset(found_snapshots, '\0', sizeof(SNAPINFO*));

    // search each found snapshot for the target file. If found, add the information
    // to the array to be returned
    int counter = 0;
    for (counter = 0; counter < snap_count; counter++) {
        search_path.timestamp = all_snapshots[counter]->detail;
        if (checkFileExists(search_path)) {
            found_snapshots = appendSNAPINFOarray(found_snapshots, all_snapshots[counter]);
        } else {
            //free the SNAPINFO if it will not be used
            free(all_snapshots[counter]);
        }
    }

    free(all_snapshots);

    return found_snapshots;
}

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

int getTotalSnapshotCount() {
    int total = 0;
    char buffer[100];
    FILE *fp;

    if(!(fp = popen("/usr/local/lib/list_snapshots.sh","r"))) {
        perror("Cannot open base path info\n");
        exit(-1);
    }

    while ((fgets(buffer, 100,fp)) != NULL) {
        total++;
    }

    if((pclose(fp)) == -1) {
        perror("cant close file stream...");
        exit(-1);
    }

    total = total / 2;

    return total;
}

SNAPINFO **getSnapshotInfo(int num_snapshots) {
    char buffer[100];
    FILE *fp;

    SNAPINFO **all_snapshots = NULL;

    EXIT_IF_NULL((all_snapshots = (SNAPINFO**)malloc((num_snapshots+1)*sizeof(SNAPINFO*))),
            "ERROR: system_info::getSnapshotInfo(): cannot allocate memory for snapshots.");
    memset(all_snapshots, '\0', (num_snapshots+1)*sizeof(SNAPINFO*));

    if(!(fp = popen("/usr/local/lib/list_snapshots.sh","r"))) {
        perror("Cannot open base path info\n");
        exit(-1);
    }

    // loop through each snapshot (filtered and detailed) and store their information into
    // the newly allocated snapinfo array.
    int counter = 0;
    while ((fgets(buffer, 100,fp)) != NULL) {
        char *detail, *summary;

        detail = strdup(buffer);

        if((fgets(buffer, 100,fp)) == NULL) {
            perror("Unable to read from snapshot list\n");
            exit(-1);
        }
        summary = strdup(buffer);

        all_snapshots[counter] = createSNAPINFO(detail, summary);
        counter++;
    }

    if((pclose(fp)) == -1) {
        perror("cant close file stream...");
        exit(-1);
    }

    all_snapshots[counter] = NULL;

    return all_snapshots;
}

SNAPINFO *createSNAPINFO(char *detail, char *summary) {
    SNAPINFO* return_val = NULL;

    // return NULL if any of the input values are empty or null
    if (!detail || strlen(detail) == 0) {
        return NULL;
    } else if (!summary || strlen(summary) == 0) {
        return NULL;
    }

    return_val = (SNAPINFO*)malloc(sizeof(SNAPINFO));
    memset(return_val, '\0', sizeof(SNAPINFO));

    return_val->detail = detail;
    return_val->summary = summary;

    return return_val;
}

//TODO: setup tests for this function
SNAPINFO **appendSNAPINFOarray(SNAPINFO **current_array, SNAPINFO *new_SNAPINFO) {
    //if either input is null, null is returned
    if (!new_SNAPINFO || !current_array) {
        return NULL;
    }

    int current_len = 0;
    while(current_array[current_len]) {
        current_len++;
    }
    //offset for the new entry and the terminating NULL
    int new_len = current_len + 2;

    SNAPINFO **new_array = (SNAPINFO**)malloc(new_len*sizeof(SNAPINFO*));
    memset(new_array, '\0', new_len*sizeof(SNAPINFO*));

    //copy over the existing SNAPINFO's into the newly allocated array
    int counter = 0;
    while(current_array[counter]) {
        new_array[counter] = current_array[counter];
        counter++;
    }
    new_array[counter] = new_SNAPINFO;

    free(current_array);

    return new_array;
}

bool checkFileExists(FULLPATH path) {
    // compile the filepath from the individual parts
    int full_len = strlen(path.base) + strlen(path.timestamp) + strlen(path.input_file);
    char *file_to_check = (char*)malloc((full_len+1)*sizeof(char));
    memset(file_to_check, '\0', (full_len+1)*sizeof(char));

    strncpy(file_to_check, path.base, strlen(path.base));
    strncat(file_to_check, path.timestamp, strlen(path.timestamp));
    strncat(file_to_check, path.input_file, strlen(path.input_file));

    struct stat buffer;

    bool ret_val = true;

    if (stat (file_to_check, &buffer)) {
        ret_val = false;
    }

    free(file_to_check);

    return ret_val;
}
