#include <stdlib.h>

#include <check.h>

#include "tests.h"

int main(void) {
	SRunner *runner = srunner_create(create_main_suite());

	srunner_run_all(runner, CK_NORMAL);

	int failed_count = srunner_ntests_failed(runner);

	srunner_free(runner);

	return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
