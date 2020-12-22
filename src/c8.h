#ifndef C8_H_INCLUDED
#define C8_H_INCLUDED

#include <stdint.h>
#include <stddef.h>

#include "c8_random.h"

// expose for testing
struct C8_T {
    uint8_t       Vx[16];
    uint16_t      I;
    uint8_t       DT;
    uint8_t       ST;
    uint16_t      PC;
    uint8_t       SP;
    uint16_t      Stack[16];
    uint8_t      *Ram;
    C8_Random_ptr Random;
};

typedef struct C8_T *C8_ptr;

extern C8_ptr   C8_init         (C8_Random_ptr rnd);
extern void     C8_free         (C8_ptr *c8);

extern void     C8_reset        (C8_ptr c8);
extern void     C8_load_program (C8_ptr c8, const uint8_t *data, size_t n);
extern void     C8_cycle        (C8_ptr c8);
extern void     C8_timers       (C8_ptr c8);

#endif // C8_H_INCLUDED