#ifndef C8_APP_H_INCLUDED
#define C8_APP_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct C8_App;

typedef struct C8_App *C8_App_ptr;

extern C8_App_ptr  C8_App_init(const char *game_path, size_t fps); // TODO settings
extern void        C8_App_free(C8_App_ptr *app);

extern void        C8_App_main_loop(C8_App_ptr app);

#ifdef __cplusplus
}
#endif


#endif // C8_APP_H_INCLUDED