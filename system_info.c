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

