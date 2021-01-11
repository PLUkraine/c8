#ifndef   C8_PRIVATE_H_INCLUDED
#define   C8_PRIVATE_H_INCLUDED

#include "c8.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const uint16_t C8_DIGIT_SIZE;
extern const uint16_t C8_SPRITE_WIDTH;


extern void    C8_exec_opcode       (C8_ptr c8, uint16_t opcode);
extern uint8_t C8_is_waiting_for_key(C8_ptr c8);
extern void    C8_set_key_lock      (C8_ptr c8, uint8_t reg);
extern void    C8_remove_key_lock   (C8_ptr c8, uint8_t key);

#ifdef __cplusplus
}
#endif

#endif // C8_PRIVATE_H_INCLUDED