#include "c8_app.h"

#include <SDL2/SDL.h>
#include <assert.h>
#include <errno.h>
#include "cummon.h"
#include "logging.h"
#include "c8.h"
#include "c8_read.h"

#define APP_WIDTH (C8_DISPLAY_WIDTH * 10)
#define APP_HEIGHT (C8_DISPLAY_HEIGHT * 10)
#define EXIT_BAD 1
#define TIMER_FREQ (1.0f/60.0f)
#define UPDATE_FREQ (1.0f/200.0f)


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
    exit(EXIT_BAD);
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

    // swap buffers -> show picture
    SDL_RenderPresent(renderer);
}


void load_c8_program(C8_ptr c8, const char *filepath)
{
    const size_t N = C8_LAST_ADDR - C8_START_ADDR + 1;
    uint8_t opcodes[N];
    size_t read_bytes = 0;
    if ((read_bytes = C8_read_program(filepath, opcodes, N)) == 0) {
        log_error("Couldn't read game data. Make sure that file \"%s\" exists and it is less than %d bytes",
            filepath,
            N);
        log_error("ERRNO: %s", strerror(errno));
        exit(EXIT_BAD);
    }

    C8_reset(c8);
    C8_load_program(c8, opcodes, read_bytes);
}


float compute_delta(Uint64 *start_time)
{
    Uint64 end_time = SDL_GetPerformanceCounter();
    float delta = (float) (end_time - *start_time) / SDL_GetPerformanceFrequency();
    *start_time = end_time;
    return delta;
}


void process_events(bool *quit)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            *quit = true;
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

    // init, reset and load program
    app->rng = C8_Random_new(42); // todo: init from time
    app->disp = C8_Display_init();
    app->c8 = C8_init(app->rng, app->disp);
    load_c8_program(app->c8, game_path);

    // init SDL rendering
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
    bool quit = false;
    Uint64 start_time = SDL_GetPerformanceCounter();
    float timer_accum = 0.0;
    float update_accum = 0.0;
    
    while (!quit)
    {
        float delta = compute_delta(&start_time);
        log_info("Delta time: %f\n", delta);

        process_events(&quit);

        timer_accum  += delta;
        while (timer_accum  > TIMER_FREQ) {
            timer_accum -= TIMER_FREQ;
            C8_timers(app->c8);
        }

        update_accum += delta;
        while (update_accum > UPDATE_FREQ) {
            update_accum -= UPDATE_FREQ;
            C8_cycle(app->c8);
        }
        
        render_c8_disp(app->renderer, app->disp);
    }
}
