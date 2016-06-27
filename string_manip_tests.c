/* Alexaner Simchuk, string_manip_tests.c
 * Unit test infrastructure for testing the methods in string_manip.c
 */

#include "string_manip.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

bool run_stringArrayLen = true;

#define ASSERT_ISNULL(val) (!val) ? true : false
#define ASSERT_TRUE(val) (val) ? true : false
#define ASSERT_FALSE(val) (val) ? false : true
#define ASSERT_STR_EQUAL(str1,str2) (strcmp(str1,str2) == 0) ? true: false
#define ASSERT_STR_NEQUAL(str1,str2) (strcmp(str1,str2) != 0) ? true: false

#define PSUC() GRN "Success!" RESET
#define PFAIL() RED "Failed." RESET
#define PYEL(str) YEL str RESET
#define PMAG(str) MAG str RESET
#define PCYN(str) CYN str RESET

int main(int argc, char* argv[]) {

    //Tests for string_manip.c::stringArrayLen()
    if (run_stringArrayLen) {
        printf(PYEL("TESTING STRINGARRAYLEN():\n"));
        printf(PMAG("(all should pass)\n"));

        char **test1 = NULL;
        printf("%s is NULL input == -1... %s\n", PCYN("Testing stringArrayLen():"),
                (-1 == stringArrayLen(test1)) ? PSUC() : PFAIL());

        char *test2[] = { NULL };
        printf("%s is {%s} == 0 ... %s\n", PCYN("Testing stringArrayLen():"),
                test2[0],
                (0 == stringArrayLen(test2)) ? PSUC() : PFAIL());

        char *test3[] = { "string1", NULL };
        printf("%s is {%s, %s} == 1 ... %s\n", PCYN("Testing stringArrayLen():"),
                test3[0], test3[1],
                (1 == stringArrayLen(test3)) ? PSUC() : PFAIL());

        char *test4[] = { "string1", "string2", NULL };
        printf("%s is {%s, %s, %s} == 2 ... %s\n", PCYN("Testing stringArrayLen():"),
                test4[0], test4[1], test4[2],
                (2 == stringArrayLen(test4)) ? PSUC() : PFAIL());
    }

    return 0;
}
