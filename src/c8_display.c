#include "c8_display.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"

static const int C8_DISPLAY_SIZE = C8_DISPLAY_WIDTH * C8_DISPLAY_HEIGHT;

struct C8_Display
{
    uint8_t *pixels;
};

int coord_1d(int row, int col, int width)
{
    return row * width + col;
}

C8_Display_ptr C8_Display_init(void)
{
    void   *malloc_call = NULL;
    C8_Display_ptr disp = NULL;

    malloc_call = malloc(sizeof(*disp));
    assert(malloc_call);
    disp = (C8_Display_ptr) malloc_call;

    malloc_call = malloc(C8_DISPLAY_SIZE);
    assert(malloc_call);
    disp->pixels = (uint8_t *) malloc_call;

    return disp;
}

void C8_Display_free(C8_Display_ptr *disp)
{
    assert(disp && *disp);
    free(*disp);
    *disp = NULL;
}

C8_Display_Color C8_Display_pixel(C8_Display_ptr disp, int row, int col)
{
    assert(disp);
    assert(0<=row && row<C8_DISPLAY_HEIGHT);
    assert(0<=col && col<C8_DISPLAY_WIDTH);

    return disp->pixels[coord_1d(row, col, C8_DISPLAY_WIDTH)];
}

C8_Display_Color C8_Display_pixel_toggle(C8_Display_ptr disp, int row, int col)
{
    assert(disp);
    assert(0<=row && row<C8_DISPLAY_HEIGHT);
    assert(0<=col && col<C8_DISPLAY_WIDTH);

    uint8_t *pixel = disp->pixels + coord_1d(row, col, C8_DISPLAY_WIDTH);
    return *pixel = !(*pixel);
}

void C8_Display_clear(C8_Display_ptr disp)
{
    assert(disp);

    memset(disp->pixels, C8_DISPLAY_OFF, C8_DISPLAY_SIZE);
}

bool C8_Display_is_clear(C8_Display_ptr disp)
{
    assert(disp);

    int i;
    C8_Display_Color result = C8_DISPLAY_OFF;
    for (i=0; i<C8_DISPLAY_SIZE; ++i)
    {
        result |= disp->pixels[i];
    }
    return !result;
}