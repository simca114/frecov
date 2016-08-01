/* Alexander Simchuk, path_interpreter.h
 *
 *
 *
 */

#ifndef _PATH_INTERPRETER_H
  #define _PATH_INTERPRETER_H

  #include "validation_macros.h"
  #include "system_info.h"
  #include <ctype.h>
  #include <stdbool.h>

  char *genSearchPath(char *user_input);

  char *getCurrentUser();
  char *getCurrentDistro();

  char *getPathType(char *path);

  bool validAbsHome(char *home, char *user, char *distro);
  bool validAbsCat(char *u, char *user);

  char *concatPath(char **ordered_path);
  char **splitPath(char *path);

  //TODO: char **getMenuItems(SNAPINFO **snapshot_list);
#endif
