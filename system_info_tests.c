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
#define ASSERT_NUM_EQUAL(num1,num2) (num1 == num2) ? true: false
#define ASSERT_STR_EQUAL(str1,str2) (strcmp(str1,str2) == 0) ? true: false
#define ASSERT_STR_NEQUAL(str1,str2) (strcmp(str1,str2) != 0) ? true: false

#define PSUC() GRN "Success!" RESET
#define PFAIL() RED "Failed." RESET
#define PYEL(str) YEL str RESET
#define PMAG(str) MAG str RESET
#define PCYN(str) CYN str RESET

bool run_getCurrentUser = true;
bool run_getCurrentDistro = true;
bool run_getBasePath = true;
bool run_getSnapshotCount = true;
bool run_getSnapshotInfo = true;
bool run_checkFileExists = true;

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

    if (run_getBasePath) {
        printf(PYEL("TESTING GETBASEPATH():\n"));
        printf(PMAG("(only one of these should pass)\n"));
        printf(PMAG("(string_manip:stripNewline() used, should pass first)\n"));

        char *gbp_real = "/rvolumes/onion/06/.zfs/snapshot";

        char *gbp_ret_val = getBasePath();
        printf("%s is returned not null... %s\n", PCYN("Testing getBasePath():"),
                (ASSERT_ISNOTNULL(gbp_ret_val)) ? PSUC() : PFAIL());
        printf("%s is returned string '%s' == '%s'... %s\n", PCYN("Testing getBasePath():"),
                gbp_ret_val, gbp_real,
                (ASSERT_STR_EQUAL(gbp_real, gbp_ret_val)) ? PSUC() : PFAIL());

        free(gbp_ret_val);
    }

    if (run_getSnapshotCount) {
        printf(PYEL("TESTING GETSNAPSHOTCOUNT():\n"));
        printf(PMAG("(this may or may not pass depending on how the current snapshots are configured)\n"));

        int gsc_current = 34;

        int gsc_ret_count = getSnapshotCount();

        printf("%s is returned int (%d == %d)... %s\n", PCYN("Testing getSnapshotCount():"),
                gsc_ret_count, gsc_current,
                (ASSERT_NUM_EQUAL(gsc_current, gsc_ret_count)) ? PSUC() : PFAIL());
    }

    if (run_getSnapshotInfo) {
        printf(PYEL("TESTING GETSNAPSHOTINFO():\n"));
        printf(PMAG("(this may or may not pass depending on how the current snapshots are configured)\n"));
        printf(PMAG("(system_info:run_getSnapshotCount() used, should pass first)\n"));

        //TODO: setup a set of static files to test against

        int gsi_count = getSnapshotCount();
        SNAPINFO **gsi_ret_val = NULL;
        gsi_ret_val = getSnapshotInfo(gsi_count);

        printf("%s with arguement (%d)\n", PCYN("Testing getSnapshotCount():"),
                gsi_count);
        printf("Is returned value not null... %s\n",
                (ASSERT_ISNOTNULL(gsi_ret_val)) ? PSUC() : PFAIL());

        bool gsi_no_nulls = true;

        int counter = 0;
        for (counter = 0; counter < gsi_count; counter++) {
            if (!gsi_ret_val[counter]->detail || !gsi_ret_val[counter]->summary || !gsi_ret_val[counter]) {
                gsi_no_nulls = false;
            }
        }

        printf("No null in any of the SNAPINFO's (SNAPINFO's included)... %s\n",
                (gsi_no_nulls) ? PSUC() : PFAIL());

        // free up memory used
        counter = 0;
        for (counter = 0; counter < gsi_count; counter++) {
            free(gsi_ret_val[counter]->detail);
            free(gsi_ret_val[counter]->summary);
            free(gsi_ret_val[counter]);
        }
        free(gsi_ret_val);
    }

    if (run_checkFileExists) {
        printf(PYEL("TESTING CHECKFILEEXISTS():\n"));
        printf(PMAG("(all should pass)\n"));
        printf(PMAG("(Tests are run expecting user simca to have these files)\n"));

        FULLPATH cfe_test;
        cfe_test.base = "/home/simca";
        cfe_test.timestamp = "/ubuntu";
        cfe_test.input_file = "/scripts/main.cpp";

        //TEST 1, file that actually exists
        bool cfe_result = checkFileExists(cfe_test);

        printf("%s with FULLPATH.base = %s, .timestamp = %s, .input_file = %s\n",
                PCYN("Testing checkFileExists():"),
                cfe_test.base, cfe_test.timestamp, cfe_test.input_file);
        printf("returned value is true...%s\n",
                (ASSERT_TRUE(cfe_result)) ? PSUC() : PFAIL());

        cfe_test.input_file = "/scripts/mainnnn.cpp";
        cfe_result = checkFileExists(cfe_test);

        printf("%s with FULLPATH.base = %s, .timestamp = %s, .input_file = %s\n",
                PCYN("Testing checkFileExists():"),
                cfe_test.base, cfe_test.timestamp, cfe_test.input_file);
        printf("returned value is false...%s\n",
                (ASSERT_FALSE(cfe_result)) ? PSUC() : PFAIL());
    }

    return 0;
}
