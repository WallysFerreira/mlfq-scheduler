#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_something(void) {
    CU_ASSERT_EQUAL(1, 1);
}

int suite_init(void) {
    return 0;
}

int suite_cleanup(void) {
    return 0;
}

int get_error(void) {
    CU_cleanup_registry();
    return CU_get_error();
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("Config_Parser_Tests", suite_init, suite_cleanup);
    if (suite == NULL) {
        return get_error();
    }

    if ((CU_add_test(suite, "Something should do somethig", test_something) == NULL)) {
        return get_error();
    };

    CU_basic_run_tests();

    CU_cleanup_registry();

    return 0;
}
