/* Alexander Simchuk, string_manip.c
 *
 * Definitions for the string manipulation functions defined in
 * string_manip.h
 *
 */

#include "string_manip.h"

// TODO: revise function name
// TODO: revise function description
/* This method will manipulate the given array by removing the given "amount"
 * of elements from the beginning of the array. (REVISE!!!)
 *
 * Memory will be allocated for the returned array and will have to be freed by the caller.
 *
 * INPUT: char** of array to manipulate
 *        int number of elements to drop
 *
 * RETURN: newly allocatted array with the specified prefix indeces removed,
 *         Null if error
 */
char **removeBegArray(char **input, int amount) {

    if (!input) {
        return NULL;
    }
    int input_length = stringArrayLen(input);
    if (input_length == 0) {
        return NULL;
    }
    if (input_length < amount) {
        return NULL;
    }

    // the new array should have the length of the existing one minus the amount to
    // be removed, plus 1 for the terminating NULL value.
    char **new_array = (char**)malloc((input_length-amount+1)* sizeof(char*));

    // transfer the existing char*'s from the existing array to the knew one
    int ctr = 0;
    for(ctr = 0; ctr < input_length-amount; ctr++) {
        new_array[ctr] = input[ctr+amount];
        //new_array[ctr] = (char*)malloc((strlen(input[ctr+amount])+1) * sizeof(char));
        //strncpy(new_array[ctr], input[ctr+amount], strlen(input[ctr+amount]));
    }
    new_array[ctr] = NULL;

    return new_array;
}

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
        while(array[len] != NULL) {
            len++;
        }
    }

    return len;
}

/* This method checks whether the end of the string ends with a newline and strips it if it
 * finds one.
 *
 * INPUT: char* to have its end checked 
 *
 * RETURN: original string without a trailing newline if it had one
 *  NULL if input string is null or input string is empty
 */
char *stripNewline(char *input) {
    if (!input) {
        return NULL;
    } else if (strlen(input) == 0) {
        return NULL;
    }

    if (input[strlen(input)-1] == '\n') {
        input[strlen(input)-1] = '\0';
    }

    return input;
}
