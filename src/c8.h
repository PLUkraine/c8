#ifndef C8_INCLUDED
#define C8_INCLUDED

#include <stdint.h>
#include <stddef.h>

#define T C8_T
typedef struct T *T;

extern T        C8_init         (void);
extern void     C8_free         (T *c8);

extern void     C8_reset        (void);
extern void     C8_load_program (const uint8_t *data, size_t n);
extern void     C8_cycle        (void);

#undef T
#endif