#ifndef C8_DISPLAY_H_INCLUDED
#define C8_DISPLAY_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

#define C8_DISPLAY_WIDTH 64
#define C8_DISPLAY_HEIGHT 32

#ifdef __cplusplus
extern "C" {
#endif

struct C8_Display;
typedef struct C8_Display *C8_Display_ptr;

typedef enum {
    C8_DISPLAY_OFF = 0,
    C8_DISPLAY_ON  = 1,
} C8_Display_Color;

extern C8_Display_ptr      C8_Display_init(void);
extern void                C8_Display_free(C8_Display_ptr *disp);

extern C8_Display_Color    C8_Display_pixel(C8_Display_ptr disp, int row, int col);
extern C8_Display_Color    C8_Display_pixel_toggle(C8_Display_ptr disp, int row, int col);
extern void                C8_Display_clear(C8_Display_ptr disp);
extern bool                C8_Display_is_clear(C8_Display_ptr disp);

extern int coord_1d(int row, int col, int width);

#ifdef __cplusplus
}
#endif

#endif