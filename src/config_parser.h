#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "types.h"

typedef enum {
    PARSE_OK = 0,
    PARSE_ERROR = 1,
    PARSE_EMPTY_PROCESSES_ERROR = 2,
    PARSE_NO_PROCESSES_FIELD_ERROR = 3
} ParseResultCode;

ParseResultCode parse(const char* data, Config *config);

#endif