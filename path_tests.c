/* Alexaner Simchuk, path_tests.c
 * Unit test infrastructure for testing the methods in path_interpreter.c
 */

#include "validation_macros.h"
#include "system_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

bool run_getBasePath = true;
bool run_getPathType = true;
bool run_validAbsHome = true;
bool run_validAbsCat = true;
bool run_concatPath = true;
bool run_splitPath = true;

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

int checkSuccess(bool test) {
    if (test) {
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char* argv[]) {
    int total_tests = 0;
    int tests_passed = 0;

    if (run_getBasePath) {
        // implemintations for testing this function have been omitted from this public repository to hide sensative information
    }

    //Tests for path_interpreter.c::getPathType()
    if (run_getPathType) {
        printf(PYEL("TESTING GETPATHTYPE(INPUT):\n"));
        printf(PMAG("(all should pass)\n"));

        char *garbage = NULL;

        printf("%s \"NULL\" is NULL... %s\n", PCYN("Testing getPathType(path):"),
                (ASSERT_ISNULL(getPathType(NULL))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(getPathType(NULL)));

        printf("%s \"\" is NULL... %s\n", PCYN("Testing getPathType(path):"),
                (ASSERT_ISNULL(getPathType(""))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(getPathType("")));

        char *abs_hom = "/home/simca/ubuntu/vamos";
        printf("%s \"%s\" is abs_hom... %s\n", PCYN("Testing getPathType(path):"), abs_hom,
                (ASSERT_STR_EQUAL("abs_hom", garbage = getPathType(abs_hom))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_STR_EQUAL("abs_hom", garbage = getPathType(abs_hom)));

        if (garbage) {
            free(garbage);
            garbage = NULL;
        }

        char *abs_cat = "/u/simca/vamos";
        printf("%s \"%s\" is abs_cat... %s\n", PCYN("Testing getPathType(path):"), abs_cat,
                (ASSERT_STR_EQUAL("abs_cat", garbage = getPathType(abs_cat))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_STR_EQUAL("abs_cat", garbage = getPathType(abs_cat)));

        if (garbage) {
            free(garbage);
            garbage = NULL;
        }

        char *rel_hom = "~/vamos";
        printf("%s \"%s\" is rel_hom... %s\n", PCYN("Testing getPathType(path):"), rel_hom,
                (ASSERT_STR_EQUAL("rel_hom", garbage = getPathType(rel_hom))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_STR_EQUAL("rel_hom", garbage = getPathType(rel_hom)));

        if (garbage) {
            free(garbage);
            garbage = NULL;
        }

        char *rel_cwd = "vamos";
        printf("%s \"%s\" is rel_cwd... %s\n", PCYN("Testing getPathType(path):"), rel_cwd,
                (ASSERT_STR_EQUAL("rel_cwd", garbage = getPathType(rel_cwd))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_STR_EQUAL("rel_cwd", garbage = getPathType(rel_cwd)));

        if (garbage) {
            free(garbage);
            garbage = NULL;
        }
    }

    //Tests for path_interpreter.c::validAbsHome()
    if (run_validAbsHome) {
        printf(PYEL("TESTING VALIDABSHOME(HOME,USER,DISTRO):\n"));
        printf(PMAG("(GETCURRENTUSER() should all pass first)\n"));
        printf(PMAG("(all should pass)\n"));

        char *test[] = {NULL, NULL, NULL};
        printf("%s is validAbsHome(NULL, NULL, NULL) is NULL... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                (ASSERT_ISNULL(validAbsHome(test[0], test[1], test[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(validAbsHome(test[0], test[1], test[2])));

        char *test1[] = {"home", NULL, "ubuntu"};
        printf("%s is validAbsHome(\"%s\", NULL, \"%s\") is NULL... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test1[0], test1[2],
                (ASSERT_ISNULL(validAbsHome(test1[0], test1[1], test1[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(validAbsHome(test1[0], test1[1], test1[2])));

        char *test2[] = {"home", "simca", NULL};
        printf("%s is validAbsHome(\"%s\", \"%s\", NULL) is NULL... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test2[0], test2[1],
                (ASSERT_ISNULL(validAbsHome(test2[0], test2[1], test2[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(validAbsHome(test2[0], test2[1], test2[2])));

        char *test3[] = {NULL, "simca", "ubuntu"};
        printf("%s is validAbsHome(NULL, \"%s\", \"%s\") is NULL... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test3[1], test3[2],
                (ASSERT_ISNULL(validAbsHome(test3[0], test3[1], test3[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(validAbsHome(test3[0], test3[1], test3[2])));

        char *test4[] = {"home", "simca", "ubuntu"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") true... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test4[0], test4[1], test4[2],
                (ASSERT_TRUE(validAbsHome(test4[0], test4[1], test4[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_TRUE(validAbsHome(test4[0], test4[1], test4[2])));

        char *test5[] = {"hom", "simca", "ubuntu"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") false... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test5[0], test5[1], test5[2],
                (ASSERT_FALSE(validAbsHome(test5[0], test5[1], test5[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_FALSE(validAbsHome(test5[0], test5[1], test5[2])));

        char *test6[] = {"", "simca", "ubuntu"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") false... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test6[0], test6[1], test6[2],
                (ASSERT_FALSE(validAbsHome(test6[0], test6[1], test6[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_FALSE(validAbsHome(test6[0], test6[1], test6[2])));

        char *test7[] = {"home", "sia", "ubuntu"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") false... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test7[0], test7[1], test7[2],
                (ASSERT_FALSE(validAbsHome(test7[0], test7[1], test7[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_FALSE(validAbsHome(test7[0], test7[1], test7[2])));

        char *test8[] = {"home", "", "ubuntu"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") false... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test8[0], test8[1], test8[2],
                (ASSERT_FALSE(validAbsHome(test8[0], test8[1], test8[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_FALSE(validAbsHome(test8[0], test8[1], test8[2])));

        char *test9[] = {"home", "simca", "solaris"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") true... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test9[0], test9[1], test9[2],
                (ASSERT_TRUE(validAbsHome(test9[0], test9[1], test9[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_TRUE(validAbsHome(test9[0], test9[1], test9[2])));

        char *test10[] = {"home", "simca", "redhat5"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") true... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test10[0], test10[1], test10[2],
                (ASSERT_TRUE(validAbsHome(test10[0], test10[1], test10[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_TRUE(validAbsHome(test10[0], test10[1], test10[2])));

        char *test11[] = {"home", "simca", "redhat6"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") true... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test11[0], test11[1], test11[2],
                (ASSERT_TRUE(validAbsHome(test11[0], test11[1], test11[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_TRUE(validAbsHome(test11[0], test11[1], test11[2])));

        char *test12[] = {"home", "simca", "common"};
        printf("%s is validAbsHome(\"%s\", \"%s\", \"%s\") true... %s\n",
                PCYN("Testing validAbsHome(home, user, distro):"),
                test12[0], test12[1], test12[2],
                (ASSERT_TRUE(validAbsHome(test12[0], test12[1], test12[2]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_TRUE(validAbsHome(test12[0], test12[1], test12[2])));
    }

    //Tests for path_interpreter.c::validAbsCat()
    if (run_validAbsCat) {
        printf(PYEL("TESTING VALIDABSCAT(U,USER):\n"));
        printf(PMAG("(GETCURRENTUSER() should all pass first)\n"));
        printf(PMAG("(all should pass)\n"));

        char *test[] = {NULL, NULL};
        printf("%s is validAbsCat(NULL, NULL) is NULL... %s\n",
                PCYN("Testing validAbsCat(u, user):"),
                (ASSERT_ISNULL(validAbsCat(test[0], test[1]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(validAbsCat(test[0], test[1])));

        char *test1[] = {NULL, "simca"};
        printf("%s is validAbsCat(NULL, \"%s\") is NULL... %s\n",
                PCYN("Testing validAbsCat(u, user):"),
                test1[1],
                (ASSERT_ISNULL(validAbsCat(test1[0], test1[1]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(validAbsCat(test1[0], test1[1])));

        char *test2[] = {"u", NULL};
        printf("%s is validAbsCat(\"%s\", NULL) is NULL... %s\n",
                PCYN("Testing validAbsCat(u, user):"),
                test2[0],
                (ASSERT_ISNULL(validAbsCat(test2[0], test2[1]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(validAbsCat(test2[0], test2[1])));

        char *test3[] = {"u", "simca"};
        printf("%s is validAbsCat(\"%s\", \"%s\") true... %s\n",
                PCYN("Testing validAbsCat(u, user):"),
                test3[0], test3[1],
                (ASSERT_TRUE(validAbsCat(test3[0], test3[1]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_TRUE(validAbsCat(test3[0], test3[1])));

        char *test4[] = {"us", "simca"};
        printf("%s is validAbsCat(\"%s\", \"%s\") false... %s\n",
                PCYN("Testing validAbsCat(u, user):"),
                test4[0], test4[1],
                (ASSERT_FALSE(validAbsCat(test4[0], test4[1]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_FALSE(validAbsCat(test4[0], test4[1])));

        char *test5[] = {"", "simca"};
        printf("%s is validAbsCat(\"%s\", \"%s\") false... %s\n",
                PCYN("Testing validAbsCat(u, user):"),
                test5[0], test5[1],
                (ASSERT_FALSE(validAbsCat(test5[0], test5[1]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_FALSE(validAbsCat(test5[0], test5[1])));

        char *test6[] = {"u", "simcass"};
        printf("%s is validAbsCat(\"%s\", \"%s\") false... %s\n",
                PCYN("Testing validAbsCat(u, user):"),
                test6[0], test6[1],
                (ASSERT_FALSE(validAbsCat(test6[0], test6[1]))) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_FALSE(validAbsCat(test6[0], test6[1])));

    }

    //Tests for path_interpreter.c::concatPath()
    if (run_concatPath) {
        printf(PYEL("TESTING CONCATPATH(ORDERED_PATH):\n"));
        printf(PMAG("(all should pass)\n"));

        char cp_str1[] = "path1";
        char cp_str2[] = "path2";
        char cp_str3[] = "path3";
        char cp_str4[] = "path4";
        char cp_str5[] = "path5";
        char cp_str6[] = "path6";
        //test 1
        char **cp_input = (char**)malloc(4*sizeof(char*));
        cp_input[0] = cp_str1;
        cp_input[1] = cp_str2;
        cp_input[2] = cp_str3;
        cp_input[3] = NULL;
        char cp_res1[] = "/path1/path2/path3";

        char *cp_result = concatPath(cp_input);

        printf("%s using arg {%s, %s, %s, %s}:\n",
                PCYN("Testing concatPath(ordered_path):"),
                cp_str1, cp_str2, cp_str3, "NULL");
        printf("Return value is not NULL ... %s\n",
                (ASSERT_ISNOTNULL(cp_result)) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNOTNULL(cp_result));
        printf("New array \"%s\" is \"%s\" ... %s\n", cp_result, cp_res1,
                (strcmp(cp_result, cp_res1) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(cp_result, cp_res1) == 0);
        if (cp_result) {
            free(cp_result);
            cp_result = NULL;
        }
        if (cp_input) {
            free(cp_input);
            cp_input = NULL;
        }

        //test 2
        cp_input = (char**)malloc(7*sizeof(char*));
        cp_input[0] = cp_str1;
        cp_input[1] = cp_str2;
        cp_input[2] = cp_str3;
        cp_input[3] = cp_str4;
        cp_input[4] = cp_str5;
        cp_input[5] = cp_str6;
        cp_input[6] = NULL;
        char cp_res2[] = "/path1/path2/path3/path4/path5/path6";

        cp_result = concatPath(cp_input);

        printf("%s using arg {%s, %s, %s, %s, %s, %s, %s}:\n",
                PCYN("Testing concatPath(ordered_path):"),
                cp_str1, cp_str2, cp_str3, cp_str4, cp_str5, cp_str6, "NULL");
        printf("Return value is not NULL ... %s\n",
                (ASSERT_ISNOTNULL(cp_result)) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNOTNULL(cp_result));
        printf("New array \"%s\" is \"%s\" ... %s\n", cp_result, cp_res2,
                (strcmp(cp_result, cp_res2) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(cp_result, cp_res2) == 0);
        if (cp_result) {
            free(cp_result);
            cp_result = NULL;
        }
        if (cp_input) {
            free(cp_input);
            cp_input = NULL;
        }

        //test 3
        cp_input = NULL;
        cp_result = concatPath(cp_input);

        printf("%s using arg %s:\n",
                PCYN("Testing concatPath(ordered_path):"),
                "NULL");
        printf("Return value is NULL ... %s\n",
                (ASSERT_ISNULL(cp_result)) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(cp_result));

        //test 4
        cp_input = (char**)malloc(1*sizeof(char*));
        cp_input[0] = NULL;

        cp_result = concatPath(cp_input);
        printf("%s using arg {%s}:\n",
                PCYN("Testing concatPath(ordered_path):"),
                "NULL");
        printf("Return value is NULL ... %s\n",
                (ASSERT_ISNULL(cp_result)) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(cp_result));

    }

    //Tests for path_interpreter.c::concatPath()
    if (run_splitPath) {
        printf(PYEL("TESTING SPLITPATH(PATH):\n"));
        printf(PMAG("(all should pass)\n"));

        //test1
        char sp_test1[] = "/with/a/starting/slash\0";
        char *sp_cmp1[] = { "with", "a", "starting", "slash", NULL };

        printf("%s using arg \"%s\":\n",
                PCYN("Testing splitPath(path):"), sp_test1);
        char **sp_result = splitPath(sp_test1);

        printf("Resulting array index 0 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[0], sp_cmp1[0],
                (strcmp(sp_result[0], sp_cmp1[0]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[0], sp_cmp1[0]) == 0);
        printf("Resulting array index 1 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[1], sp_cmp1[1],
                (strcmp(sp_result[1], sp_cmp1[1]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[1], sp_cmp1[1]) == 0);
        printf("Resulting array index 2 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[2], sp_cmp1[2],
                (strcmp(sp_result[2], sp_cmp1[2]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[2], sp_cmp1[2]) == 0);
        printf("Resulting array index 3 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[3], sp_cmp1[3],
                (strcmp(sp_result[3], sp_cmp1[3]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[3], sp_cmp1[3]) == 0);
        printf("Resulting array index 4 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[4], sp_cmp1[4],
                (ASSERT_ISNULL(sp_result[4])) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(sp_result[4]));

        if (sp_result) {
            free(sp_result);
            sp_result = NULL;
        }

        //test 2
        char sp_test2[] = "with/ending/slash/\0";
        char *sp_cmp2[] = { "with", "ending", "slash", NULL };

        printf("%s using arg \"%s\":\n",
                PCYN("Testing splitPath(path):"), sp_test2);
        sp_result = splitPath(sp_test2);

        printf("Resulting array index 0 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[0], sp_cmp2[0],
                (strcmp(sp_result[0], sp_cmp2[0]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[0], sp_cmp2[0]) == 0);
        printf("Resulting array index 1 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[1], sp_cmp2[1],
                (strcmp(sp_result[1], sp_cmp2[1]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[1], sp_cmp2[1]) == 0);
        printf("Resulting array index 2 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[2], sp_cmp2[2],
                (strcmp(sp_result[2], sp_cmp2[2]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[2], sp_cmp2[2]) == 0);
        printf("Resulting array index 3 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[3], sp_cmp2[3],
                (ASSERT_ISNULL(sp_result[3])) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(sp_result[3]));

        if (sp_result) {
            free(sp_result);
            sp_result = NULL;
        }

        //test 3
        char sp_test3[] = "/with/both/slash/beginning/and/ending/\0";
        char *sp_cmp3[] = { "with", "both", "slash", "beginning", "and", "ending", NULL };

        printf("%s using arg \"%s\":\n",
                PCYN("Testing splitPath(path):"), sp_test3);
        sp_result = splitPath(sp_test3);

        printf("Resulting array index 0 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[0], sp_cmp3[0],
                (strcmp(sp_result[0], sp_cmp3[0]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[0], sp_cmp3[0]) == 0);
        printf("Resulting array index 1 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[1], sp_cmp3[1],
                (strcmp(sp_result[1], sp_cmp3[1]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[1], sp_cmp3[1]) == 0);
        printf("Resulting array index 2 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[2], sp_cmp3[2],
                (strcmp(sp_result[2], sp_cmp3[2]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[2], sp_cmp3[2]) == 0);
        printf("Resulting array index 3 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[3], sp_cmp3[3],
                (strcmp(sp_result[3], sp_cmp3[3]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[3], sp_cmp3[3]) == 0);
        printf("Resulting array index 4 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[4], sp_cmp3[4],
                (strcmp(sp_result[4], sp_cmp3[4]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[4], sp_cmp3[4]) == 0);
        printf("Resulting array index 5 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[5], sp_cmp3[5],
                (strcmp(sp_result[5], sp_cmp3[5]) == 0) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(strcmp(sp_result[5], sp_cmp3[5]) == 0);
        printf("Resulting array index 6 value \"%s\" is == \"%s\" ... %s\n",
                sp_result[6], sp_cmp3[6],
                (ASSERT_ISNULL(sp_result[6])) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(sp_result[6]));

        if (sp_result) {
            free(sp_result);
            sp_result = NULL;
        }

        //test 4
        printf("%s using arg \"%s\":\n",
                PCYN("Testing splitPath(path):"), "NULL");
        sp_result = splitPath(NULL);
        printf("Result is %s ... %s\n",
                "NULL",
                (ASSERT_ISNULL(sp_result)) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(sp_result));

        //test 5
        char sp_test5[] = "";
        printf("%s using arg \"%s\":\n",
                PCYN("Testing splitPath(path):"), sp_test5);
        sp_result = splitPath(sp_test5);
        printf("Result is %s ... %s\n",
                "NULL",
                (ASSERT_ISNULL(sp_result)) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(sp_result));

        //test 6
        char sp_test6[] = "/";
        printf("%s using arg \"%s\":\n",
                PCYN("Testing splitPath(path):"), sp_test6);
        sp_result = splitPath(sp_test6);
        printf("Result is %s ... %s\n",
                "NULL",
                (ASSERT_ISNULL(sp_result)) ? PSUC() : PFAIL());
        total_tests++;
        tests_passed += checkSuccess(ASSERT_ISNULL(sp_result));
    }

    printf("%s %d/%d\n", PMAG("Passed Tests:"),
            total_tests, tests_passed);

    return 0;
}
