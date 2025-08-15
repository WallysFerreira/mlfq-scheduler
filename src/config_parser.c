#include <stdio.h>
#include <cjson/cJSON.h>
#include "types.h"
#include "config_parser.h"

ParseErrorCode generic_error(const char *error_message) {
    fprintf(stderr, "%s\n", error_message);
    return PARSE_ERROR;
}

ParseErrorCode error(const char *error_message, ParseErrorCode errorCode) {
    fprintf(stderr, "%s\n", error_message);
    return errorCode;
}

int parse_process_item(cJSON *processItemJson, Config *config) {
    cJSON *processIdJson = cJSON_GetObjectItem(processItemJson, "id");
    if (cJSON_IsNumber(processIdJson)) {
        printf("Process id = %d\n", processIdJson->valueint);
        config->id = processIdJson->valueint;
        return PARSE_OK;
    } else {
        return generic_error("Expected \"id\" field to be a number");
    }
}

ParseErrorCode parse_processes_array(cJSON *jsonRoot, Config *config) {
    cJSON *processesJson = cJSON_GetObjectItem(jsonRoot, "processes");
    if (cJSON_IsArray(processesJson)) {
        if (cJSON_GetArraySize(processesJson) == 0) {
            return error("\"processes\" should not be empty", PARSE_EMPTY_PROCESSES_ERROR);
        }

        for (int i = 0; i < cJSON_GetArraySize(processesJson); i++) {
            cJSON *processItemJson = cJSON_GetArrayItem(processesJson, i);
            if (processItemJson == NULL) {
                char *error_msg;
                sprintf(error_msg, "Item not found at index %d", i);
                return generic_error(error_msg);
            }

            return parse_process_item(processItemJson, config);
        }
    } else {
        return generic_error("Expected \"processes\" field to be an array");
    }
}

ParseErrorCode parse(const char* data, Config *config) {
    cJSON *jsonRoot = cJSON_Parse(data);
    if (jsonRoot == NULL) {
        return generic_error("Something went wrong when parsing json");
    }

    ParseErrorCode result = parse_processes_array(jsonRoot, config);

    cJSON_Delete(jsonRoot);
    return result;
}