/* Alexander Simchuk, frecov.c, Version 2.0
   This is a curses application to allow unix users to access their zfs snapshots and copy the
   contents to their homedir.
*/

#include "frecov.h"

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printUsageMessageThenExit();
    }

    if( strcmp(argv[1],"--help") == 0 || strcmp(argv[1],"-h") == 0 ) {
        printHelpMessage();
        exit(-1);
    }

    SNAPINFO **found_snapshots;
    char *base, *input_file;

    base = getBasePath();
    input_file = genSearchPath(argv[1]);

    found_snapshots = searchSnapshotsForFile(base, input_file);

    char **menu_options = getSnapshotMenuOptions(found_snapshots);
    int num_options = stringArrayLen(menu_options);

    if (num_options == 0) {
        printNoBackupFound();
        exit(0);
    }

    //print menu for user to make a selection
    int choice = mainMenu("Available versions", menu_options, num_options);
    system("clear");

    if (choice >= 0) {

        FULLPATH file_to_recover;
        file_to_recover.base = base;
        file_to_recover.timestamp = found_snapshots[choice]->detail;
        file_to_recover.input_file = input_file;

        char *dest_dir = getDestDir(argv[1], found_snapshots[choice]->summary);
        createDestDirIfDoesntExist(dest_dir);

        if (copyBackup(file_to_recover, dest_dir)) {
            printInstructions();
        } else {
            perror("ERROR: could not copy file(s) from backup.");
            exit(-1);
        }
    }

    //free up used memory
    int counter = 0;
    while (menu_options[counter]) {
        free(menu_options[counter]);
        counter++;
    }
    free(menu_options);

    counter = 0;
    while (found_snapshots[counter]) {
        free(found_snapshots[counter]->detail);
        free(found_snapshots[counter]->summary);
        free(found_snapshots[counter]);
        counter++;
    }
    free(found_snapshots);

    free(input_file);
    free(base);

    return 0;
}
