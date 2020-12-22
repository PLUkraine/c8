#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "cummon.h"
#include "c8.h"
#include "c8_private.h"

static const uint16_t C8_START_ADDR = 0x200;
static const uint16_t C8_LAST_ADDR  = 0xFFF;

static const uint8_t C8_DIGITS[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80,
};

// 3.5795 MHz?
C8_ptr C8_init(C8_Random_ptr rnd)
{
    assert(rnd);

    void *malloc_call = NULL;
    C8_ptr c8 = NULL;

    malloc_call = malloc(sizeof(*c8));
    assert(malloc_call);
    c8 = (C8_ptr) malloc_call;

    malloc_call = malloc(C8_LAST_ADDR + 1);
    assert(malloc_call);
    c8->Ram = (uint8_t *)malloc_call;

    c8->Random = rnd;

    return c8;
}

void C8_free(C8_ptr *c8)
{
    assert(c8 && *c8);
    free((*c8)->Ram);
    free(*c8);
    *c8 = NULL;
}

void C8_reset(C8_ptr c8)
{
    assert(c8);
    c8->DT = 0;
    c8->I = 0;
    c8->PC = C8_START_ADDR;
    c8->SP = 0;
    c8->ST = 0;
    memset(c8->Vx, 0x00, NELEMS(c8->Vx));
    memcpy(c8->Ram, C8_DIGITS, NELEMS(C8_DIGITS));
}

void C8_load_program (C8_ptr c8, const uint8_t *data, size_t n)
{
    assert(c8);
    assert(data);
    assert(0 < n && n <= (size_t)(C8_LAST_ADDR - C8_START_ADDR + 1));
    memcpy(c8->Ram + C8_START_ADDR, data, n);
}

void C8_cycle (C8_ptr c8)
{
    assert(c8);
    assert(!(c8->PC & 1));
    
    uint16_t opcode = (c8->Ram[c8->PC] << 8) | c8->Ram[c8->PC+1];
    c8->PC += 2;
    C8_exec_opcode(c8, opcode);
}

void C8_timers(C8_ptr c8)
{
    assert(c8);
    if (c8->DT != 0) c8->DT--;
    if (c8->ST != 0) c8->ST--;
}
