#ifndef C8_APP_H_INCLUDED
#define C8_APP_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

struct C8_App;

typedef struct C8_App *C8_App_ptr;

extern C8_App_ptr  C8_App_init(void); // TODO settings
extern void        C8_App_free(C8_App_ptr *app);

extern void        C8_App_main_loop(C8_App_ptr app);


#endif // C8_APP_H_INCLUDED