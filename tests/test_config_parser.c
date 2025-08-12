#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/types.h"
#include "../src/config_parser.h"

void test_processes_info_parsing(void) {
    char* json_content = "{\"processes\":[{\"id\":1,\"arrival\":0,\"burst\":10}]}";
    int expected_id = 1;

    Config actual_config = parse(json_content);

    if (actual_config.id != expected_id) {
        printf("Expected id %d, got %d\n", expected_id, actual_config.id);
        CU_FAIL();
    }
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

    if ((CU_add_test(suite, "Should correctly parse process data", test_processes_info_parsing) == NULL)) {
        return get_error();
    };

    CU_basic_run_tests();

    CU_cleanup_registry();

    return 0;
}
