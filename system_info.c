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

//TODO:SNAPINFO** searchSnapshots(FULLPATH path);

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

int getSnapshotCount() {
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

    if(!(fp = popen("/usr/local/lib/list_snapshots.sh","r"))) {
        perror("Cannot open base path info\n");
        exit(-1);
    }

    // loop through each snapshot (filtered and detailed) and store their information into
    // the newly allocated snapinfo array.
    int counter = 0;
    while ((fgets(buffer, 100,fp)) != NULL) {
        SNAPINFO *temp = NULL;

        EXIT_IF_NULL((temp = (SNAPINFO*)malloc(sizeof(SNAPINFO))),
                "ERROR: system_info::getSnapshotInfo(): cannot allocate memory for temporary snapshot.");

        //TODO: check for null
        temp->detail = strdup(buffer);

        if((fgets(buffer, 100,fp)) == NULL) {
            perror("Unable to read from snapshot list\n");
            exit(-1);
        }
        //TODO: check for null
        temp->summary = strdup(buffer);

        all_snapshots[counter] = temp;
        counter++;
    }

    if((pclose(fp)) == -1) {
        perror("cant close file stream...");
        exit(-1);
    }

    all_snapshots[counter] = NULL;

    return all_snapshots;
}

//TODO:bool setSNAPINFO(char *detail, char *summary);
//TODO:bool checkFileExists(FULLPATH path);
