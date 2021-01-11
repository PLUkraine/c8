#include "c8_keyboard.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "cummon.h"


struct C8_Keyboard
{
    uint8_t key[C8_KEYBOARD_SIZE];
    uint8_t *Vx;
};


C8_Keyboard_ptr C8_Keyboard_init(void)
{
    void *malloc_call = NULL;
    C8_Keyboard_ptr keys  = NULL;

    malloc_call = malloc(sizeof(*keys));
    assert(malloc_call);
    keys = (C8_Keyboard_ptr) malloc_call;

    memset(keys->key, C8_KEY_UP, NELEMS(keys->key));
    keys->Vx = NULL;

    return keys;
}


void C8_Keyboard_free(C8_Keyboard_ptr *keys)
{
    assert(keys && *keys);
    free(*keys);
    *keys = NULL;
}


void C8_Keyboard_set(C8_Keyboard_ptr keys, uint8_t key, C8_Key_State state)
{
    assert(keys);
    assert(key < NELEMS(keys->key));

    keys->key[key] = state;
    if (keys->Vx && state) {
        *(keys->Vx) = key;
        keys->Vx = NULL;
    }
}


C8_Key_State C8_Keyboard_get(C8_Keyboard_ptr keys, uint8_t key)
{
    assert(keys);
    assert(key < NELEMS(keys->key));

    return keys->key[key];
}


void C8_Keyboard_lock(C8_Keyboard_ptr keys, uint8_t *Vx)
{
    assert(keys);
    assert(Vx);

    keys->Vx = Vx;
}


bool C8_Keyboard_is_locked(C8_Keyboard_ptr keys)
{
    assert(keys);

    return keys->Vx != NULL;
}
