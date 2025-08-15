#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "types.h"

typedef enum {
    PARSE_OK = 0,
    PARSE_ERROR = 1
} ParseErrorCode;

ParseErrorCode parse(const char* data, Config *config);

#endif