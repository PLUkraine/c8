#ifndef C8_DISPLAY_H_INCLUDED
#define C8_DISPLAY_H_INCLUDED

#include <stdint.h>

struct C8_Display;
typedef struct C8_Display *C8_Display_ptr;

extern const uint16_t C8_DISPLAY_WIDTH;
extern const uint16_t C8_DISPLAY_HEIGHT;

C8_Display_ptr C8_Display_init();
void           C8_Display_free(C8_Display_ptr *disp);

uint8_t        C8_Display_pixel(C8_Display_ptr disp, uint8_t row, uint8_t col);
uint8_t        C8_Display_pixel_toggle(C8_Display_ptr disp, uint8_t row, uint8_t col);
uint8_t        C8_Display_clear(C8_Display_ptr disp);

#endif