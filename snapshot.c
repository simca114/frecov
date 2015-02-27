/* Alexander Simchuk, snapshot.c, Version 1.0
  This is a curses application to allow unix users to access their zfs snapshots and copy the
  contents to their homedir.
*/
#include "autoCurseMenu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define EXIT_IF_NULL(var,...) do {  \
  if(!var)                          \
  {                                 \
    fprintf(stderr,__VA_ARGS__);    \
    exit(-1);                       \
  }                                 \
}while(0)

#define EXIT_IF_NONZERO(var,...) do { \
  if(var)                             \
  {                                   \
    fprintf(stderr,__VA_ARGS__);      \
    exit(-1);                         \
  }                                   \
}while(0)

void printInstructions();

int main() {

  int cntr, num_items, choice;

  FILE *fp;
  char buffer[50], **choices_references, **choices_main, *base_path, *user, *choice_path;

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
    base_path = strdup(buffer);
    if(base_path[(strlen(base_path)-1)] == '\n') {
      base_path[(strlen(base_path)-1)] = '\0';
    }
  }

  if((fgets(buffer, sizeof(buffer),fp)) != NULL) {
    user = strdup(buffer);
    if(user[(strlen(user)-1)] == '\n') {
      user[(strlen(user)-1)] = '\0';
    }
  }

  cntr = 0;

  while (fgets(buffer, sizeof(buffer),fp) != NULL) {
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
  pclose(fp);

  choice = mainMenu("Please Select a Date",choices_main,num_items);

  if(choice >= 0) {

    printInstructions();

    choice_path = (char*)calloc(strlen(base_path) +
                                strlen(choices_references[choice]) +
                                strlen(user) +
                                2,sizeof(char));

          strcpy(choice_path,base_path);
          strcat(choice_path,"/");
          strcat(choice_path,choices_references[choice]);
          strcat(choice_path,"/");
          strcat(choice_path,user);

    system("clear");

    if(choice_path != NULL) {
      if((chdir(choice_path)) != 0) {
        perror("Failed to change directory");
        exit(-1);
      }

      execl("/bin/bash","",NULL);
    }
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
