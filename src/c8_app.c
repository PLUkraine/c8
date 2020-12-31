#include "c8_app.h"

#include <SDL2/SDL.h>
#include <assert.h>
#include "cummon.h"
#include "c8.h"

// static const int APP_WIDTH = (const int)C8_DISPLAY_WIDTH * 10;
// static const int APP_HEIGHT = (const int)C8_DISPLAY_HEIGHT * 10;


struct C8_App
{
    SDL_Window *window;
};


C8_App_ptr C8_App_init(void)
{
    void   *malloc_call = NULL;
    C8_App_ptr app = NULL;

    malloc_call = malloc(sizeof(*app));
    assert(malloc_call);
    app = (C8_App_ptr) malloc_call;

    return app;
}

void C8_App_free(C8_App_ptr *app)
{
    Q_UNUSED(app);
}

void C8_App_main_loop(C8_App_ptr app)
{
    Q_UNUSED(app);
}
