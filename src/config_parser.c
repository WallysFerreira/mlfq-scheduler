#include <stdio.h>
#include <cjson/cJSON.h>
#include "types.h"
#include "config_parser.h"

ParseResultCode error(const char *error_message, ParseResultCode errorCode) {
    fprintf(stderr, "%s\n", error_message);
    return errorCode;
}

ParseResultCode parse_process_item(cJSON *processItemJson, Config *config) {
    cJSON *processIdJson = cJSON_GetObjectItem(processItemJson, "id");
    if (cJSON_IsNumber(processIdJson)) {
        printf("Process id = %d\n", processIdJson->valueint);
        config->id = processIdJson->valueint;
        return PARSE_OK;
    } else {
        return error("Expected \"id\" field to be a number", PARSE_ID_NOT_NUMBER_ERROR);
    }
}

ParseResultCode parse_processes_array(cJSON *jsonRoot, Config *config) {
    cJSON *processesJson = cJSON_GetObjectItem(jsonRoot, "processes");
    if (processesJson == NULL) {
        return error("\"processes\" field should exist", PARSE_NO_PROCESSES_FIELD_ERROR);
    }

    if (cJSON_IsArray(processesJson)) {
        if (cJSON_GetArraySize(processesJson) == 0) {
            return error("\"processes\" should not be empty", PARSE_EMPTY_PROCESSES_ERROR);
        }

        for (int i = 0; i < cJSON_GetArraySize(processesJson); i++) {
            cJSON *processItemJson = cJSON_GetArrayItem(processesJson, i);
            return parse_process_item(processItemJson, config);
        }
    } else {
        return error("Expected \"processes\" field to be an array", PARSE_PROCESSES_NOT_ARRAY_ERROR);
    }
}

ParseResultCode parse(const char* data, Config *config) {
    cJSON *jsonRoot = cJSON_Parse(data);
    if (jsonRoot == NULL) {
        return error("Something went wrong when parsing json", PARSE_INVALID_JSON_ERROR);
    }

    ParseResultCode result = parse_processes_array(jsonRoot, config);

    cJSON_Delete(jsonRoot);
    return result;
}