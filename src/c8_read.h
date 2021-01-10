#ifndef C8_READ_H_INCLUDED
#define C8_READ_H_INCLUDED

#include <stdint.h>
#include <unistd.h>

extern size_t C8_read_program(const char *path, uint8_t *buffer, size_t max_size);

#endif // C8_READ_H_INCLUDED