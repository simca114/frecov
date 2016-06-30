/* Alexander Simchuk, print_messages.h
 *
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _PRINT_MESSAGES_H
  #define _PRINT_MESSAGES_H

  void printHelpMessage();
  void printInstructions();
  void printUsageMessageThenExit();
  void printPathExampleThenExit(char *user);

#endif
