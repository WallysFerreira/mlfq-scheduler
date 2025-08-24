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

ParseResultCode parse_process_id(cJSON *process_item_json, Config *config) {
    cJSON *process_id_json = cJSON_GetObjectItem(process_item_json, "id");
    if (cJSON_IsNumber(process_id_json)) {
        printf("Process id = %d\n", process_id_json->valueint);
        config->id = process_id_json->valueint;
        return PARSE_OK;
    } else {
        return error("Expected \"id\" field to be a number", PARSE_ID_NOT_NUMBER_ERROR);
    }
}

ParseResultCode parse_process_arrival(cJSON *process_item_json, Config *config) {
    cJSON *arrival_json = cJSON_GetObjectItem(process_item_json, "arrival");
    if (cJSON_IsNumber(arrival_json)) {
        printf("Process arrival = %d\n", arrival_json->valueint);
        config->arrival = arrival_json->valueint;
        return PARSE_OK;
    } else {
        return error("Expected \"arrival\" field to be a number", PARSE_ERROR);
    }
}

ParseResultCode parse_process_item(cJSON *processItemJson, Config *config) {
    RETURN_IF_ERROR(parse_process_id(processItemJson, config));
    RETURN_IF_ERROR(parse_process_arrival(processItemJson, config));
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