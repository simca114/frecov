/* Alexander Simchuk, snapshot.h
 *
 *
 */

#ifndef _SNAPSHOT_H
  #define _SNAPSHOT_H

  #include "autoCurseMenu.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <errno.h>

  #define EXIT_IF_NULL(var,...) do {  \
    if(!var) {                        \
      fprintf(stderr,__VA_ARGS__);    \
      exit(-1);                       \
    }                                 \
  }while(0)

  #define EXIT_IF_NONZERO(var,...) do { \
    if(var) {                         \
      fprintf(stderr,__VA_ARGS__);    \
      exit(-1);                       \
    }                                 \
  }while(0)

  typedef struct {
    char *base;
    char *timestamp;
    char *input_file;
  } FULLPATH;

  int fileInDir(const FULLPATH *testpath);

  void printInstructions();

#endif
