/* Alexander Simchuk, snapshot.c, Version 1.0
  This is a curses application to allow unix users to access their zfs snapshots and copy the
  contents to their homedir.
*/
#include "snapshot.h"

//int fileInDir(char * path,char * dir,);

int main(int argc, char * argv[]) {

  if(argc != 2) {
    printf("Usage: snapshot $FILE\n");
    exit(-1);
  }

  FULLPATH current_path;

  current_path.input_file = interpretPath(argv[1]);

  int cntr, num_items, choice;

  FILE *fp;
  char buffer[50], **choices_references, **choices_main;

  //initialize char array for main menu options
  if(!(fp = popen("./list_dates.sh","r"))) {
    perror("Cannot access list of dates\n");
    exit(-1);
  }

  cntr = 0;
  while ((fgets(buffer, sizeof(buffer),fp)) != NULL) {
    cntr++;
  }
  pclose(fp);

  num_items = (cntr - 2) / 2;

  choices_main = (char **)calloc(num_items,sizeof(char *));
  choices_references = (char **)calloc(num_items,sizeof(char *));

  if(!(fp = popen("./list_dates.sh","r"))) {
    perror("Cannot access list of dates\n");
    exit(-1);
  }

  if((fgets(buffer, sizeof(buffer),fp)) != NULL) {
    current_path.base = strdup(buffer);
    if(current_path.base[(strlen(current_path.base)-1)] == '\n') {
      current_path.base[(strlen(current_path.base)-1)] = '\0';
    }
  }

  cntr = 0;

  char path_buffer[PATH_MAX];
  struct stat temp;

  while (fgets(buffer, sizeof(buffer),fp) != NULL) {

    memset(path_buffer,0,PATH_MAX*sizeof(char));
    errno = 0;
    snprintf(path_buffer,PATH_MAX*sizeof(char),"%s%s%s",current_path.base,buffer,current_path.input_file);
    if( errno ) { perror("snprintf"); }

    if( (stat(path_buffer,&temp)) == 0 ) {
        choices_references[cntr] = strdup(buffer);

        if (choices_references[cntr][(strlen(choices_references[cntr])-1)] == '\n') {
            choices_references[cntr][(strlen(choices_references[cntr])-1)] = '\0';
        }

        fgets(buffer, sizeof(buffer),fp);
        choices_main[cntr] = strdup(buffer);

        if (choices_main[cntr][(strlen(choices_main[cntr])-1)] == '\n') {
            choices_main[cntr][(strlen(choices_main[cntr])-1)] = '\0';
        }
        ++cntr;
    }
  }
  pclose(fp);

  if( cntr == 0) {
    exit(-1);
  }

  choice = mainMenu("Please Select a Date",choices_main,num_items);

  if(choice >= 0) {

    printInstructions();


    system("clear");

  }

  system("clear");

  return 0;
}

void printInstructions() {
    int X,Y;

    Y = ((LINES - 10)/2);
    X = ((COLS - 42)/2);

    refresh();

    init_pair(5,COLOR_BLACK,COLOR_WHITE);

    curs_set(1);
    attron(COLOR_PAIR(5));
    mvprintw(Y,X,  "                                          ");
    mvprintw(Y+1,X," You are now in a new shell inside the    ");
    mvprintw(Y+2,X," directory of the date you selected. Use  ");
    mvprintw(Y+3,X," the command 'cp' to copy over the files  ");
    mvprintw(Y+4,X," of interest. When you are finised,       ");
    mvprintw(Y+5,X," use the 'exit' command to leave this     ");
    mvprintw(Y+6,X," shell and return to the one you were in. ");
    mvprintw(Y+7,X,"                                          ");
    mvprintw(Y+8,X," Press any key to continue...             ");
    mvprintw(Y+9,X,"                                          ");
    move(Y+8,X+30);
    attroff(COLOR_PAIR(5));
    refresh();
    getch();
    endwin();
}
