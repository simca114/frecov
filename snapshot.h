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
  #include <sys/stat.h>

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

  void printInstructions();

  //path checking functions
  int fileInDir(const FULLPATH *test_path);

  char * interpretPath(char *user_input);
  void printPathExampleThenExit(char *user);

#endif
