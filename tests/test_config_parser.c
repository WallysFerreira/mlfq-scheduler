#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/types.h"
#include "../src/config_parser.h"

void test_processes_info_parsing(void) {
    char* json_content = "{\"processes\":[{\"id\":1,\"arrival\":0,\"burst\":10}]}";
    Config config;
    int expected_id = 1;

    ParseResultCode result = parse(json_content, &config);

    CU_ASSERT_EQUAL(result, PARSE_OK);
    if (config.id != expected_id) {
        printf("Expected id %d, got %d\n", expected_id, config.id);
        CU_FAIL();
    }
}

void test_empty_processes(void) {
    char* json_content = "{\"processes\":[]}";
    Config config;

    ParseResultCode result = parse(json_content, &config);

    CU_ASSERT_EQUAL(result, PARSE_EMPTY_PROCESSES_ERROR);
}

void test_no_processes_field(void) {
    char* json_content = "{\"other_field\": \"other_value\"}";
    Config config;

    ParseResultCode result = parse(json_content, &config);

    CU_ASSERT_EQUAL(result, PARSE_NO_PROCESSES_FIELD_ERROR);
}

void test_processes_not_array(void) {
    char *json_content = "{\"processes\": \"shouldBeArray\"}";
    Config config;

    ParseResultCode result = parse(json_content, &config);

    CU_ASSERT_EQUAL(result, PARSE_PROCESSES_NOT_ARRAY_ERROR);
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

    if ((CU_add_test(suite, "should correctly parse process data", test_processes_info_parsing) == NULL) ||
        (CU_add_test(suite, "should return error on empty processes list", test_empty_processes) == NULL) ||
        (CU_add_test(suite, "should return error when processes field is not found", test_no_processes_field) == NULL) ||
        (CU_add_test(suite, "should return error when processes field is not array", test_processes_not_array) == NULL)) {
        return get_error();
    };

    CU_basic_run_tests();

    CU_cleanup_registry();

    return 0;
}
