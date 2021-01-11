#ifndef C8_KEYBOARD_H_INCLUDED
#define C8_KEYBOARD_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

#define C8_KEYBOARD_SIZE 16

typedef enum {
    C8_KEY_UP = 0,
    C8_KEY_DOWN = 1,
} C8_Key_State;

#ifdef __cplusplus
extern "C" {
#endif

struct C8_Keyboard;

typedef struct C8_Keyboard* C8_Keyboard_ptr;

extern C8_Keyboard_ptr C8_Keyboard_init(void);
extern            void C8_Keyboard_free(C8_Keyboard_ptr *keys);

extern            void C8_Keyboard_set(C8_Keyboard_ptr keys, uint8_t key, C8_Key_State state);
extern    C8_Key_State C8_Keyboard_get(C8_Keyboard_ptr keys, uint8_t key);
extern            void C8_Keyboard_lock(C8_Keyboard_ptr keys, uint8_t *Vx);
extern            bool C8_Keyboard_is_locked(C8_Keyboard_ptr keys);

#ifdef __cplusplus
}
#endif


#endif // C8_KEYBOARD_H_INCLUDED