#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/types.h"
#include "../src/config_parser.h"

void assert_equals(int actual, int expected, const char *field_name) {
    if (actual != expected) {
        printf("FAIL: Expected %s %d, got %d\n", field_name, expected, actual);
        CU_FAIL();
    }
}

void test_processes_info_parsing(void) {
    char* json_content = "{\"processes\":[{\"id\":1,\"arrival\":100,\"burst\":10}]}";
    Config config;
    int expected_id = 1;
    int expected_arrival = 100;
    int expected_burst = 10;

    ParseResultCode result = parse(json_content, &config);

    CU_ASSERT_EQUAL(result, PARSE_OK);
    assert_equals(config.id, expected_id, "id");
    assert_equals(config.arrival, expected_arrival, "arrival");
    assert_equals(config.burst, expected_burst, "burst");
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

void test_invalid_json(void) {
    char *not_json_content = "this is not a valid json";
    Config config;

    ParseResultCode result = parse(not_json_content, &config);

    CU_ASSERT_EQUAL(result, PARSE_INVALID_JSON_ERROR);
}

void test_id_is_not_number(void) {
    char *json_content = "{\"processes\":[{\"id\":\"not a number\"}]}";
    Config config;

    ParseResultCode result = parse(json_content, &config);

    CU_ASSERT_EQUAL(result, PARSE_FIELD_NOT_NUMBER_ERROR);
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
        (CU_add_test(suite, "should return error when processes field is not array", test_processes_not_array) == NULL) ||
        (CU_add_test(suite, "should return error on invalid json", test_invalid_json) == NULL) ||
        (CU_add_test(suite, "should return error when id is not number", test_id_is_not_number) == NULL)) {
        return get_error();
    };

    CU_basic_run_tests();

    CU_cleanup_registry();

    return 0;
}
