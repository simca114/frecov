/* Alexaner Simchuk, path_tests.c
 * Unit test infrastructure for testing the methods in path_interpreter.c
 */

#include "system_info.h"
#include <stdbool.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define ASSERT_ISNULL(val) (!val) ? true : false
#define ASSERT_ISNOTNULL(val) (val != NULL) ? true : false
#define ASSERT_TRUE(val) (val) ? true : false
#define ASSERT_FALSE(val) (val) ? false : true
#define ASSERT_STR_EQUAL(str1,str2) (strcmp(str1,str2) == 0) ? true: false
#define ASSERT_STR_NEQUAL(str1,str2) (strcmp(str1,str2) != 0) ? true: false

#define PSUC() GRN "Success!" RESET
#define PFAIL() RED "Failed." RESET
#define PYEL(str) YEL str RESET
#define PMAG(str) MAG str RESET
#define PCYN(str) CYN str RESET

bool run_getCurrentUser = true;
bool run_getCurrentDistro = true;

int main(int argc, char* argv[]) {

    //Tests for path_interpreter.c::getCurrentUser()
    if (run_getCurrentUser) {
        printf(PYEL("TESTING GETCURRENTUSER():\n"));
        printf(PMAG("(all should pass if user simca is running these)\n"));
        char *user = getCurrentUser(); printf("%s is user simca... %s\n", PCYN("Testing getCurrentUser():"),
                (ASSERT_STR_EQUAL("simca",user)) ? PSUC() : PFAIL());

        printf("%s is user not sim6... %s\n", PCYN("Testing getCurrentUser():"),
                (ASSERT_STR_NEQUAL("sim6",user)) ? PSUC() : PFAIL());
    }

    //Tests for path_interpreter.c::getCurrentDistro()
    if (run_getCurrentDistro) {
        printf(PYEL("TESTING GETCURRENTDISTRO():\n"));
        printf(PMAG("(only one of these should pass)\n"));

        char *distro = getCurrentDistro();
        printf("%s is OS ubuntu... %s\n", PCYN("Testing getCurrentDistro():"),
                (ASSERT_STR_EQUAL("ubuntu", distro)) ? PSUC() : PFAIL());

        printf("%s is OS solaris... %s\n", PCYN("Testing getCurrentDistro():"),
                (ASSERT_STR_EQUAL("solaris", distro)) ? PSUC() : PFAIL());

        printf("%s is OS redhat5... %s\n", PCYN("Testing getCurrentDistro():"),
                (ASSERT_STR_EQUAL("redhat5", distro)) ? PSUC() : PFAIL());

        printf("%s is OS redhat6... %s\n", PCYN("Testing getCurrentDistro():"),
                (ASSERT_STR_EQUAL("redhat6", distro)) ? PSUC() : PFAIL());
        free(distro);
    }

    return 0;
}
