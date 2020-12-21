#include "c8_random.h"

#include <stdlib.h>
#include <assert.h>

static const uint32_t BIT_NOISE_1 = 0xB5297A4D;
static const uint32_t BIT_NOISE_2 = 0x68E31DA4;
static const uint32_t BIT_NOISE_3 = 0x1B56C4E9;

struct C8_Random
{
    uint32_t pos;
    uint32_t seed;
};

C8_Random_ptr C8_Random_new(uint32_t seed)
{
    void *malloc_call = NULL;
    C8_Random_ptr rnd  = NULL;

    malloc_call = malloc(sizeof(*rnd));
    assert(malloc_call);
    rnd = (C8_Random_ptr) malloc_call;

    rnd->pos = 0;
    rnd->seed = seed;

    return rnd;
}

void C8_Random_free(C8_Random_ptr *rnd)
{
    assert(rnd && *rnd);
    free(*rnd);
    *rnd = NULL;
}

uint32_t C8_Random_next(C8_Random_ptr rnd)
{
    uint32_t mangled = rnd->pos++;
    mangled *= BIT_NOISE_1;
    mangled += rnd->seed;
    mangled ^= (mangled >> 8);
    mangled += BIT_NOISE_2;
    mangled ^= (mangled << 8);
    mangled *= BIT_NOISE_3;
    mangled ^= (mangled >> 8);
    return mangled;
}
