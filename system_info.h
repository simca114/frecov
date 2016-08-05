/* Alexander Simchuk, system_info.h
 *
 * This is a header file functions that extract information from the operating system.
 *
 */

#ifndef _SYSTEM_INFO_H
  #define _SYSTEM_INFO_H

  #include <ctype.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include "string_manip.h"
  #include "path_interpreter.h"
  #include "validation_macros.h"

  typedef struct {
    char* detail;
    char* summary;
  } SNAPINFO;

  char *getCurrentUser();
  char *getCurrentDistro();

  SNAPINFO **searchSnapshotsForFile(char *base, char *input_file);
  char *getBasePath();
  int getTotalSnapshotCount();

  SNAPINFO **getSnapshotInfo(int num_snapshots);
  SNAPINFO *createSNAPINFO(char *detail, char *summary);
  SNAPINFO **appendSNAPINFOarray(SNAPINFO **current_array, SNAPINFO *new_SNAPINFO);

  bool checkFileExists(FULLPATH path);

  //TODO
  char **getSnapshotMenuOptions(SNAPINFO **valid_snapshots);
  //TODO
  bool copyBackup(FULLPATH path, char *dir_name);

#endif
