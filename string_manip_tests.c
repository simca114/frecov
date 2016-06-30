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
bool run_removeBegArray = true;

#define ASSERT_ISNULL(val) (!val) ? true : false
#define ASSERT_ISNOTNULL(val) (val) ? true : false
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

    //Tests for string_manip.c::removeBegArray()
    if (run_removeBegArray) {
        printf(PYEL("TESTING REMOVEBEGARRAY():\n"));
        printf(PMAG("(all should pass)\n"));
        printf(PMAG("(tests for STRINGARRAYLEN() should pass first)\n"));

        char str1[] = "str1";
        char str2[] = "str2";
        char str3[] = "str3";
        char str4[] = "str4";
        char str5[] = "str5";

        char **rba_test = (char**)calloc(3, sizeof(char*));
        rba_test[0] = str1;
        rba_test[1] = str2;
        rba_test[2] = NULL;

        //check each index
        char **result = removeBegArray(rba_test,1);
        printf("%s using args {%s, %s, %s} and 1:\n", PCYN("Testing removeBegArray():"),
                str1, str2, "NULL");
        printf("Return value is not NULL ... %s\n",
              (ASSERT_ISNOTNULL(result)) ? PSUC() : PFAIL());
        printf("New array index 0 is %s ... %s\n", str2,
              (strcmp(result[0], str2) == 0) ? PSUC() : PFAIL());
        printf("New array index 1 is %s ... %s\n", "NULL",
              (ASSERT_ISNULL(result[1])) ? PSUC() : PFAIL());

        free(result);
        free(rba_test);

        rba_test = (char**)calloc(6, sizeof(char*));
        rba_test[0] = str1;
        rba_test[1] = str2;
        rba_test[2] = str3;
        rba_test[3] = str4;
        rba_test[4] = str5;
        rba_test[5] = NULL;

        result = removeBegArray(rba_test,3);
        printf("%s using args {%s, %s, %s, %s, %s, %s} and 3:\n",
                PCYN("Testing removeBegArray():"),
                str1, str2, str3, str4, str5, "NULL");
        printf("Return value is not NULL ... %s\n",
              (ASSERT_ISNOTNULL(result)) ? PSUC() : PFAIL());
        printf("New array index 0 is %s ... %s\n", str4,
              (strcmp(result[0], str4) == 0) ? PSUC() : PFAIL());
        printf("New array index 1 is %s ... %s\n", str5,
              (strcmp(result[1], str5) == 0) ? PSUC() : PFAIL());
        printf("New array index 2 is %s ... %s\n", "NULL",
              (ASSERT_ISNULL(result[2])) ? PSUC() : PFAIL());

        free(result);
        free(rba_test);

        rba_test = NULL;
        result = removeBegArray(rba_test,3);
        printf("%s using args %s and 3:\n",
                PCYN("Testing removeBegArray():"),
                "NULL");
        printf("Return value is NULL ... %s\n",
              (ASSERT_ISNULL(result)) ? PSUC() : PFAIL());

        rba_test = (char**)calloc(1,sizeof(char*));
        rba_test[0] = NULL;
        result = removeBegArray(rba_test,3);
        printf("%s using args {%s} and 3:\n",
                PCYN("Testing removeBegArray():"),
                "NULL");
        printf("Return value is NULL ... %s\n",
              (ASSERT_ISNULL(result)) ? PSUC() : PFAIL());
        if(result) free(result);
        if(rba_test) free(rba_test);

        rba_test = (char**)calloc(3, sizeof(char*));
        rba_test[0] = str1;
        rba_test[1] = str2;
        rba_test[2] = NULL;

        result = removeBegArray(rba_test,3);
        printf("%s using args {%s, %s, %s} and 3:\n", PCYN("Testing removeBegArray():"),
                str1, str2, "NULL");
        printf("Return value is NULL ... %s\n",
              (ASSERT_ISNULL(result)) ? PSUC() : PFAIL());
        if(result) free(result);
        if(rba_test) free(rba_test);
    }

    return 0;
}
