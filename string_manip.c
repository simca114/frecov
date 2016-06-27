/* Alexander Simchuk, string_manip.c
 *
 * Definitions for the string manipulation functions defined in
 * string_manip.h
 *
 */

#include "string_manip.h"

/* This method counts the number of char arrays in a char** variable
 *
 * INPUT: char** with a terminating NULL element
 *
 * RETURN: will return one of the following integers
 *  -1 if inputted array is a NULL pointer
 *  number of arrays in pointer otherwise
 */
int stringArrayLen(char **array) {
    int len = 0;

    if(!array) {
        len = -1;
    }
    else {
        while(array[len]) {
            len++;
        }
    }

    return len;
}
