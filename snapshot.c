/* Alexander Simchuk, snapshot.c
  This is a curses application to allow unix users to access their zfs snapshots and copy the
  contents to their homedir.
*/

#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{
  int mn_choices = 0, i = 0, num_items;

  FILE *fp;
  char path[50];
  char **choices_main,*choice = NULL;

  ITEM **items_main;
  int c = 0;
  MENU *menu_main;
  WINDOW *menu_main_win;

  //initialize char array for main menu options
  if(!(fp = popen("./list_dates.sh","r")))
  {
    perror("Cannot access list of dates\n");
    exit(-1);
  }

  while (fgets(path, sizeof(path),fp) != NULL)
  {
    ++mn_choices;
  }
  pclose(fp);

  choices_main = (char **)calloc(mn_choices,sizeof(char *));

  fp = popen("./list_dates.sh","r");
  if(fp == NULL)
  {
    printf("Cannot access list of dates\n");
    exit(1);
  }
  while (fgets(path, sizeof(path),fp) != NULL)
  {
    choices_main[i] = strdup(path);
    if (choices_main[i][strlen(choices_main[i]) - 1] == '\n')
    {
      choices_main[i][strlen(choices_main[i]) - 1] = '\0';
    }
    ++i;
  } 
  pclose(fp);

  //set max counter for number of items
  num_items = (mn_choices - 2) / 2;

  //initialize curses
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);


  //create menu items
  int n = 0;
  items_main = (ITEM **)calloc(num_items, sizeof(ITEM *));
  for(i = 3; i < mn_choices; i = i+2)
  {
    items_main[n] = new_item(choices_main[i],choices_main[i - 1]);
    ++n;
  }
  items_main[num_items] = (ITEM *)NULL;

  //create menu
  menu_main = new_menu((ITEM **)items_main);
  menu_opts_off(menu_main, O_SHOWDESC);

  //create window
  menu_main_win = newwin(10, 40, 4,4);
  keypad(menu_main_win, TRUE);

  //Set main menu window and sub window
  set_menu_win(menu_main, menu_main_win);
  set_menu_sub(menu_main, derwin(menu_main_win, 6, 38, 3, 1));
  set_menu_format(menu_main, 5, 1);

  set_menu_mark(menu_main, " * ");

  //Print a border around the main window and print a title
  box(menu_main_win, 0, 0);
  print_in_middle(menu_main_win, 1, 0, 40, "Available Dates", COLOR_PAIR(1));
  mvwaddch(menu_main_win, 2, 0, ACS_LTEE);
  mvwhline(menu_main_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(menu_main_win, 2, 39, ACS_RTEE);

  //Post the menu
  post_menu(menu_main);
  wrefresh(menu_main_win);

  //display instructions
  attron(COLOR_PAIR(2));
  mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scroll down or up a page of items");
  mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F2 to Exit)");
  attroff(COLOR_PAIR(2));
  refresh();

  //manage input
  while(c != KEY_F(2))
  {
    c = wgetch(menu_main_win);
    switch(c)
    {
      case KEY_DOWN:
        menu_driver(menu_main, REQ_DOWN_ITEM);
     // wrefresh(menu_main_win);
        break;
      case KEY_UP:
        menu_driver(menu_main, REQ_UP_ITEM);
        break;
      case KEY_NPAGE:
        menu_driver(menu_main, REQ_SCR_DPAGE);
        break;
      case KEY_PPAGE:
        menu_driver(menu_main, REQ_SCR_UPAGE);
        break;
      case 10:
      {
        choice = (char*)calloc(strlen(choices_main[0]) +
                               strlen(item_description(current_item(menu_main))) +
                               strlen(choices_main[1]) +
                               2,sizeof(char));

        strcpy(choice,choices_main[0]);
        strcat(choice,"/");
        strcat(choice,item_description(current_item(menu_main)));
        strcat(choice,"/");
        strcat(choice,choices_main[1]);

        c = KEY_F(2);
      }
      break;
    }
    wrefresh(menu_main_win);
  }

  //clean up allocated memory
  unpost_menu(menu_main);
  for(i = 0; i < num_items; ++i)
  {
    free_item(items_main[i]);
  }
  free_menu(menu_main);

  refresh();

  endwin();

  system("clear");

  for(i = 0; i < mn_choices; ++i)
  {
    free(choices_main[i]);
  }
  free(choices_main);

  if(choice != NULL)
  {
    if((chdir(choice)) != 0)
    { 
      perror("Failed to change directory");
      exit(-1);
    }
    free(choice);

    execl("/bin/bash","",NULL);
  }

  return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
  int length, x, y;
  float temp;

  if(win == NULL)
  {
    win = stdscr;
  }
  getyx(win, y, x);
  if(startx != 0)
  {
    x = startx;
  }
  if(starty != 0)
  {
    y = starty;
  }
  if(width == 0)
  {
    width = 80;
  }

  length = strlen(string);
  temp = (width - length)/ 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}
