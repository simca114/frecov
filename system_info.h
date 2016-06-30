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

  char *getCurrentUser();
  char *getCurrentDistro();

#endif
