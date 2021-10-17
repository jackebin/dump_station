#include "minunit.h"
#include <ljack/inputstd.h>
#include <ljack/dbg.h>

static Inputstd *every = NULL;
static int rc = 0;

char *test_create() {

	every = Inputstd_create();
    mu_assert(every != NULL,
    	"memory error test_create");
    return NULL;
}

char *test_int() {

	printf("Enter an integer\n");
	rc = Inputstd_scan(scan_int, every);
    mu_assert(rc == 0,
    	"error test int");
    log_info("scan_int success");
    return NULL;
}

char *test_float() {

	printf("\nEnter a float\n");
	rc = Inputstd_scan(scan_flt, every);
    mu_assert(rc == 0,
    	"error test float");
    log_info("scan_flt success");
    return NULL;
}

char *test_string() {

	printf("\nWrite something silly (press '$' when done)\n");
	rc = Inputstd_scan(scan_str, every);
    mu_assert(rc == 0,
    	"error test string");
    log_info("scan_str success");
    return NULL;
}

char *test_print() {

	printf("\n");
	printf("Integer: %ld\n", every->std_int);
	printf("Float:   %lf\n", every->std_float);
	printf("\nSilly text:\n\n%s\n", every->std_string);
	return NULL;
}

char *test_destroy() {
	
	std_kill(every);
	return NULL;
}

char *all_tests() {

    mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_int);
	mu_run_test(test_float);
	mu_run_test(test_string);
	mu_run_test(test_print);
	mu_run_test(test_destroy);

    return NULL;
}	

RUN_TESTS(all_tests);
