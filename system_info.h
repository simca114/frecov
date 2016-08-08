/* Alexander Simchuk, system_info.h
 *
 * This is a header file functions that extract information from the operating system.
 *
 */

#ifndef _SYSTEM_INFO_H
  #define _SYSTEM_INFO_H

  #include <ctype.h>
  #include <stdbool.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include "string_manip.h"
  #include "print_messages.h"
  #include "validation_macros.h"


  typedef struct {
    char* detail;
    char* detail_path;
    char* summary;
  } SNAPINFO;

  typedef struct {
      char *base;
      char *timestamp;
      char *input_file;
  } FULLPATH;

  // system specific things

  char *getCurrentUser();
  char *getCurrentDistro();

  SNAPINFO **searchSnapshotsForFile(char *base, char *input_file);
  int getTotalSnapshotCount();

  SNAPINFO **getSnapshotInfo(int num_snapshots);
  SNAPINFO *createSNAPINFO(char *detail, char *summary);
  SNAPINFO **appendSNAPINFOarray(SNAPINFO **current_array, SNAPINFO *new_SNAPINFO);

  bool checkFileExists(FULLPATH path);

  //TODO
  char **getSnapshotMenuOptions(SNAPINFO **valid_snapshots);
  //TODO
  bool copyBackup(FULLPATH path, char *dir_name);

  // path specific things

  char *getBasePath();
  char *genSearchPath(char *user_input);

  char *getCurrentUser();
  char *getCurrentDistro();

  char *getPathType(char *path);

  bool validAbsHome(char *home, char *user, char *distro);
  bool validAbsCat(char *u, char *user);

  //TODO: char *concatFULLPATH(FULLPATH input);
  char *concatPath(char **ordered_path);
  char **splitPath(char *path);

  //TODO: char **getMenuItems(SNAPINFO **snapshot_list);


#endif
