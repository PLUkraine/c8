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

    C8_Random_ptr rng;
    C8_Display_ptr disp;
    C8_ptr c8;
};

void print_SDL_error_and_exit()
{
    log_error("SDL error:\n%s", SDL_GetError());
    abort();
}

void render_c8_disp(SDL_Renderer *renderer, C8_Display_ptr disp)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Rect pixel;
    pixel.w = APP_WIDTH / C8_DISPLAY_WIDTH;
    pixel.h = APP_HEIGHT / C8_DISPLAY_HEIGHT;

    for (int i=0; i<C8_DISPLAY_HEIGHT; ++i)
    {
        for (int j=0; j<C8_DISPLAY_WIDTH; ++j)
        {
            pixel.y = i * pixel.h;
            pixel.x = j * pixel.w;

            if (C8_Display_pixel(disp, i, j) == C8_DISPLAY_ON)
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            }

            SDL_RenderFillRect(renderer, &pixel);
        }
    }
}


C8_App_ptr C8_App_init(const char *game_path)
{
    void   *malloc_call = NULL;
    C8_App_ptr app = NULL;

    malloc_call = malloc(sizeof(*app));
    assert(malloc_call);
    app = (C8_App_ptr) malloc_call;

    app->rng = C8_Random_new(42); // todo: init from time
    app->disp = C8_Display_init();
    app->c8 = C8_init(app->rng, app->disp);

    app->window = SDL_CreateWindow("C8",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        APP_WIDTH,
        APP_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!app->window) print_SDL_error_and_exit();
    SDL_SetWindowTitle(app->window, game_path);

    app->renderer = SDL_CreateRenderer(app->window,
        -1,
        SDL_RENDERER_ACCELERATED);
    if (!app->window) print_SDL_error_and_exit();

    return app;
}

void C8_App_free(C8_App_ptr *p_app)
{
    assert(p_app && *p_app);
    C8_App_ptr app = *p_app;
    
    C8_free(&(app->c8));
    C8_Random_free(&(app->rng));
    C8_Display_free(&(app->disp));

    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

    free(*p_app);
    *p_app = NULL;
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

        // C8_Display_pixel_toggle(app->disp, 1, 1);

        render_c8_disp(app->renderer, app->disp);

        SDL_RenderPresent(app->renderer);
        SDL_Delay(100);
    }
}
