/* Simchuk Alexander, pathdisplay.c
 *
 * This is a program for testing the path interpretation feature of frecov.
 * It will display the process of interpreting the path that the user inputs
 * and determining the correct location of the file.
 */

#include "snapshot.h"
#include "path_interpreter.h"

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Usage: snapshot $FILE\n");
        exit(-1);
    }

    FULLPATH current_path;

    char *pathbuffer;

    current_path.base = "/home";
    printf("Base path: %s\n",current_path.base);

    printf("Input path : %s\n", argv[1]);
    current_path.input_file = genSearchPath(argv[1]);
    if(current_path.input_file) {
        printf("Search path generated: %s\n", current_path.input_file);
    }
    else {
        printf("ERROR:input path not interpretted\n");
        exit(-1);
    }

    pathbuffer = malloc((strlen(current_path.base) + strlen(current_path.input_file) + 1) * sizeof(char));

    sprintf(pathbuffer,"%s%s",current_path.base,current_path.input_file);

    struct stat temp;

    if( (stat(pathbuffer,&temp)) ) {
        printf("%s doesnt exist...\n",pathbuffer);
    }
    else {
        printf("%s exists!\n",pathbuffer);
    }

    return 0;
}
