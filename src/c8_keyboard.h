#ifndef C8_KEYBOARD_H_INCLUDED
#define C8_KEYBOARD_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

struct C8_Keyboard;

typedef struct C8_Keyboard* C8_Keyboard_ptr;

extern C8_Keyboard_ptr C8_Keyboard_init(void);
extern            void C8_Keyboard_free(C8_Keyboard_ptr *keys);

#ifdef __cplusplus
}
#endif


#endif // C8_KEYBOARD_H_INCLUDED