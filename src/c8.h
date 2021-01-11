#ifndef C8_H_INCLUDED
#define C8_H_INCLUDED

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "c8_random.h"
#include "c8_display.h"
#include "c8_keyboard.h"

#define C8_START_ADDR ((uint16_t)0x0200)
#define C8_LAST_ADDR  ((uint16_t)0x0FFF)

#ifdef __cplusplus
extern "C" {
#endif

// expose for testing
struct C8_T {
    uint8_t         Vx[16];
    uint16_t        I;
    uint8_t         DT;
    uint8_t         ST;
    uint16_t        PC;
    uint8_t         SP;
    uint16_t        Stack[16];
    uint8_t        *Ram;
    C8_Random_ptr   Random;
    C8_Display_ptr  Display;
    C8_Keyboard_ptr Keyboard;
};

typedef struct C8_T *C8_ptr;

extern C8_ptr   C8_init         (C8_Random_ptr rnd, C8_Display_ptr disp, C8_Keyboard_ptr keys);
extern void     C8_free         (C8_ptr *c8);

extern void     C8_reset        (C8_ptr c8);
extern void     C8_load_program (C8_ptr c8, const uint8_t *data, size_t n);
extern void     C8_cycle        (C8_ptr c8);
extern void     C8_timers       (C8_ptr c8);
extern void     C8_set_key      (C8_ptr c8, uint8_t key, bool is_down);

#ifdef __cplusplus
}
#endif

#endif // C8_H_INCLUDED