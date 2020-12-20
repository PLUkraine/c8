#ifndef C8_INCLUDED
#define C8_INCLUDED

#include <stdint.h>
#include <stddef.h>

struct C8_T;
typedef struct C8_T *C8_ptr;

extern C8_ptr   C8_init         (void);
extern void     C8_free         (C8_ptr *c8);

extern void     C8_reset        (void);
extern void     C8_load_program (const uint8_t *data, size_t n);
extern void     C8_cycle        (void);

#endif