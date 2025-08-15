#include <stdio.h>
#include "config_parser.h"

int main(void) {
    char* data = "{}";
    Config config;

    ParseResultCode result = parse(data, &config);
    if (result != PARSE_OK) {
        printf("Failed to parse json to Config. Error code: %d\n", result);
        return 1;
    }

    return 0;
}