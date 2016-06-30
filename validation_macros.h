/* Alexander Simchuk, validation_macros.h
 *
 *
 *
 */

#ifndef _VALIDATION_MACROS_H
  #define _VALIDATION_MACROS_H

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

  #define INVALID_PATH_NULL(var) do { \
    if(!var) {                        \
      printPathExampleThenExit();     \
    }                                 \
  }while(0)

#endif
