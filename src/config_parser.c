#include <stdio.h>
#include <cjson/cJSON.h>
#include "types.h"
#include "config_parser.h"

#define RETURN_IF_ERROR(expr)           \
    do {                                \
        ParseResultCode rc = (expr);    \
        if (rc != PARSE_OK) return rc;  \
    } while (0);

ParseResultCode error(const char *error_message, ParseResultCode error_code) {
    fprintf(stderr, "%s\n", error_message);
    return error_code;
}

ParseResultCode expected_number_error(const char *field_name) {
    char msg[50];
    sprintf(msg, "Expected \"%s\" field to be a number", field_name);
    return error(msg, PARSE_FIELD_NOT_NUMBER_ERROR);
}

ParseResultCode get_number_field(cJSON *parent_json, const char *field_name, int *result) {
    cJSON *field_json = cJSON_GetObjectItem(parent_json, field_name);
    if (cJSON_IsNumber(field_json)) {
        *result = field_json->valueint;
        return PARSE_OK;
    } else {
        return expected_number_error(field_name);
    }
}

ParseResultCode parse_process_id(cJSON *process_item_json, Config *config) {
    int process_id;
    RETURN_IF_ERROR(get_number_field(process_item_json, "id", &process_id));

    printf("Process id = %d\n", process_id);
    config->id = process_id;

    return PARSE_OK;
}

ParseResultCode parse_process_arrival(cJSON *process_item_json, Config *config) {
    int process_arrival;
    RETURN_IF_ERROR(get_number_field(process_item_json, "arrival", &process_arrival))

    printf("Process arrival = %d\n", process_arrival);
    config->arrival = process_arrival;

    return PARSE_OK;
}

ParseResultCode parse_process_burst(cJSON *process_item_json, Config *config) {
    int process_burst;
    RETURN_IF_ERROR(get_number_field(process_item_json, "burst", &process_burst));

    printf("Process burst = %d\n", process_burst);
    config->burst = process_burst;

    return PARSE_OK;
}

ParseResultCode parse_process_item(cJSON *processItemJson, Config *config) {
    RETURN_IF_ERROR(parse_process_id(processItemJson, config));
    RETURN_IF_ERROR(parse_process_arrival(processItemJson, config));
    RETURN_IF_ERROR(parse_process_burst(processItemJson, config));
}

ParseResultCode parse_processes_array(cJSON *json_root, Config *config) {
    cJSON *processes_json = cJSON_GetObjectItem(json_root, "processes");
    if (processes_json == NULL) {
        return error("\"processes\" field should exist", PARSE_NO_PROCESSES_FIELD_ERROR);
    }

    if (cJSON_IsArray(processes_json)) {
        if (cJSON_GetArraySize(processes_json) == 0) {
            return error("\"processes\" should not be empty", PARSE_EMPTY_PROCESSES_ERROR);
        }

        for (int i = 0; i < cJSON_GetArraySize(processes_json); i++) {
            cJSON *process_item_json = cJSON_GetArrayItem(processes_json, i);
            return parse_process_item(process_item_json, config);
        }
    } else {
        return error("Expected \"processes\" field to be an array", PARSE_PROCESSES_NOT_ARRAY_ERROR);
    }
}

ParseResultCode parse(const char* data, Config *config) {
    cJSON *json_root = cJSON_Parse(data);
    if (json_root == NULL) {
        return error("Something went wrong when parsing json", PARSE_INVALID_JSON_ERROR);
    }

    ParseResultCode result = parse_processes_array(json_root, config);

    cJSON_Delete(json_root);
    return result;
}