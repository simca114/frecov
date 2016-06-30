/*Alexander Simchuk, snapshot.h
 *
 *
 */

#ifndef _SNAPSHOT_H
  #define _SNAPSHOT_H

  #include "autoCurseMenu.h"
  #include <ctype.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <errno.h>
  #include <sys/stat.h>

  #define PATH_MAX 4096

  typedef struct {
    char *base;
    char *timestamp;
    char *input_file;
  } FULLPATH;

  //method definitions in info_getters.h
  int fileInDir(const FULLPATH *test_path);

#endif
