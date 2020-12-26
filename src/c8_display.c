#include "c8_display.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cummon.h"

const uint16_t C8_DISPLAY_WIDTH  = 64;
const uint16_t C8_DISPLAY_HEIGHT = 32;
const uint16_t C8_DISPLAY_SIZE = C8_DISPLAY_WIDTH * C8_DISPLAY_HEIGHT;

struct C8_Display
{
    uint8_t *pixels;
};

int coord_1d(int row, int col, int width)
{
    return row * width + col;
}

C8_Display_ptr C8_Display_init()
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
    assert(row < C8_DISPLAY_HEIGHT);
    assert(col < C8_DISPLAY_WIDTH);

    return disp->pixels[coord_1d(row, col, C8_DISPLAY_WIDTH)];
}

C8_Display_Color C8_Display_pixel_toggle(C8_Display_ptr disp, int row, int col)
{
    assert(row < C8_DISPLAY_HEIGHT);
    assert(col < C8_DISPLAY_WIDTH);

    uint8_t *pixel = disp->pixels + coord_1d(row, col, C8_DISPLAY_WIDTH);
    return *pixel = !(*pixel);
}

void C8_Display_clear(C8_Display_ptr disp)
{
    int i, j;
    for (i=0; i<C8_DISPLAY_HEIGHT; ++i)
    {
        for (j=0; j<C8_DISPLAY_WIDTH; ++j)
        {
            disp->pixels[coord_1d(i, j, C8_DISPLAY_WIDTH)] = C8_DISPLAY_OFF;
        }
    }
}
