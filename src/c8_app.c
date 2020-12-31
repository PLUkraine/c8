#include "c8_app.h"

#include <SDL2/SDL.h>
#include <assert.h>
#include "cummon.h"
#include "logging.h"
#include "c8.h"

#define APP_WIDTH (C8_DISPLAY_WIDTH * 10)
#define APP_HEIGHT (C8_DISPLAY_HEIGHT * 10)


struct C8_App
{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void print_SDL_error_and_exit()
{
    log_error("SDL error:\n%s", SDL_GetError());
    abort();
}



C8_App_ptr C8_App_init(void)
{
    void   *malloc_call = NULL;
    C8_App_ptr app = NULL;

    malloc_call = malloc(sizeof(*app));
    assert(malloc_call);
    app = (C8_App_ptr) malloc_call;

    app->window = SDL_CreateWindow("C8",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        APP_WIDTH,
        APP_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!app->window) print_SDL_error_and_exit();

    app->renderer = SDL_CreateRenderer(app->window,
        -1,
        SDL_RENDERER_ACCELERATED);
    if (!app->window) print_SDL_error_and_exit();

    return app;
}

void C8_App_free(C8_App_ptr *app)
{
    assert(app && *app);
    SDL_DestroyRenderer((*app)->renderer);
    SDL_DestroyWindow((*app)->window);
    free(*app);
    *app = NULL;
}

void C8_App_main_loop(C8_App_ptr app)
{
    //Event handler
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
        SDL_RenderClear(app->renderer);

        SDL_RenderPresent(app->renderer);
        SDL_Delay(100);
    }
}
