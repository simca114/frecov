/* Alexander Simchuk, string_manip.h
 *
 * This is a header file for string manipulation support functions
 *
 */

#ifndef _STRING_MANIP_H
  #define _STRING_MANIP_H

  #include <stdbool.h>
  #include <string.h>
  #include <stdio.h>
  #include <stdlib.h>

  char **removeBegArray(char **input, int amount);
  int stringArrayLen(char **array);

#endif
