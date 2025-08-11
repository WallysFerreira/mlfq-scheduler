#include <stdio.h>
#include "config_parser.h"

int main(void) {
    char* data = "{}";
    Config config = parse(data);
    return 0;
}