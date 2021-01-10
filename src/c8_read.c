#include "c8_read.h"

#include <stdio.h>

size_t C8_read_program(const char *path, uint8_t *buffer, size_t max_size)
{
    FILE *c8_file = NULL;
    if ((c8_file = fopen(path, "rb")) == NULL)
    {
        return 0;
    }

    size_t bytes_read;
    bytes_read = fread(buffer, 1, max_size, c8_file);
    if (!feof(c8_file) || ferror(c8_file))
    {
        return 0;
    }

    return bytes_read;
}

