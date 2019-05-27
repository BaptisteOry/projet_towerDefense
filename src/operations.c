#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/operations.h"

void slice_str(const char * str, char * buffer, int start, int end) {
    int j = 0;
    for (int i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}