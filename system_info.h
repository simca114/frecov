/* Alexander Simchuk, system_info.h
 *
 * This is a header file functions that extract information from the operating system.
 *
 */

#ifndef _SYSTEM_INFO_H
  #define _SYSTEM_INFO_H

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <ctype.h>
  #include "string_manip.h"
  #include "validation_macros.h"

  typedef struct {
    char* detail;
    char* summary;
  } SNAPINFO;

  char *getCurrentUser();
  char *getCurrentDistro();

  //TODO:SNAPINFO** searchSnapshots(FULLPATH path);
  char *getBasePath();
  int getSnapshotCount();
  SNAPINFO **getSnapshotInfo(int num_snapshots);
  //TODO:bool setSNAPINFO(char *detail, char *summary);
  //TODO:bool checkFileExists(FULLPATH path);

#endif
