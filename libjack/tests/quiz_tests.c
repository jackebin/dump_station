#include "minunit.h"

static Quiz * quiz = NULL;

const int    TEST_VALS[] = {2, 20};
const char   TEST_TITL[] = "QUIZ - TEST";
const char * TEST_ANSW[] = {"ANSW1", "ANSW2"};
const char * TEST_QUIZ[] = {"INPUT1", "INPUT2"};

char * test_quiz_create() {

    quiz = Quiz_create(TEST_VALS);
    return NULL;
}

char * test_quiz_copy ()  {

    Quiz_copy(quiz, TEST_TITL, TEST_ANSW, TEST_QUIZ);
    return NULL;
}

char * test_quiz_input() {

    Quiz_input(quiz);
    return NULL;
}

char * test_quiz_compare() {

    Quiz_compare(quiz);
    return NULL;
}

char * test_quiz_result() {

    Quiz_result(quiz);
    printf("\n");
    return NULL;
}

char * test_quiz_destroy() {

    Quiz_destroy(quiz);
    return NULL;
}

char *all_tests() {

    mu_suite_start();

    mu_run_test(test_quiz_create);
    mu_run_test(test_quiz_copy);
//  mu_run_test(test_quiz_input);
    mu_run_test(test_quiz_compare);
    mu_run_test(test_quiz_result);
    mu_run_test(test_quiz_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
