
#include "snapshot.h"

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
    current_path.input_file = interpretPath(argv[1]);
    if(current_path.input_file) {
        printf("Input path interpretted: %s\n", current_path.input_file);
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


char * interpretPath(char *user_input) {

    char *new_string, *user, *buffer, *string_parts[21];
    int counter = 0;

    EXIT_IF_NULL( (user = getenv("USER")) ,
                  "ERROR: interpretPath(): getenv user failed\n");

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
}

void printPathExampleThenExit(char *user) {
    printf("Please enter a valid absolute path (ex. /home/%s/ubuntu/$pathToFile or /u/%s/$pathToFile)\n",user,user);
    exit(-1);
}
