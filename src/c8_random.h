#ifndef C8_RANDOM_H_INCLUDED
#define C8_RANDOM_H_INCLUDED

#include <stdint.h>

struct C8_Random;

typedef struct C8_Random *C8_Random_ptr;

extern C8_Random_ptr C8_Random_new(uint32_t seed);
extern void          C8_Random_free(C8_Random_ptr *rnd);

extern uint32_t      C8_Random_next(C8_Random_ptr rnd);

#endif