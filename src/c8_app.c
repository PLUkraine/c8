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


struct C8_App
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    C8_Random_ptr rng;
    C8_Display_ptr disp;
    C8_Keyboard_ptr keys;
    C8_ptr c8;
    float update_freq;
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
 #define N (C8_LAST_ADDR - C8_START_ADDR + 1)
    uint8_t opcodes[N];
    size_t read_bytes = 0;
    if ((read_bytes = C8_read_program(filepath, opcodes, N)) == 0) {
        log_error("Couldn't read game data. Make sure that file \"%s\" exists and it is less than %d bytes",
            filepath,
            N);
        exit(EXIT_BAD);
    }

    C8_reset(c8);
    C8_load_program(c8, opcodes, read_bytes);
#undef N
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


void process_keyboard(C8_Keyboard_ptr keyboard)
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    const int keys[] = {
        SDL_SCANCODE_X, // 0x00
        SDL_SCANCODE_1, // 0x01
        SDL_SCANCODE_2, // 0x02
        SDL_SCANCODE_3, // 0x03

        SDL_SCANCODE_Q, // 0x04
        SDL_SCANCODE_W, // 0x05
        SDL_SCANCODE_E, // 0x06
        SDL_SCANCODE_A, // 0x07

        SDL_SCANCODE_S, // 0x08
        SDL_SCANCODE_D, // 0x09
        SDL_SCANCODE_Z, // 0x0A
        SDL_SCANCODE_C, // 0x0B

        SDL_SCANCODE_4, // 0x0C
        SDL_SCANCODE_R, // 0x0D
        SDL_SCANCODE_F, // 0x0E
        SDL_SCANCODE_V, // 0x0F
    };

    uint8_t i;
    for (i=0; i<NELEMS(keys); ++i)
    {
        C8_Keyboard_set(keyboard, i, keystates[keys[i]]);
    }
}


C8_App_ptr C8_App_init(const char *game_path, size_t fps)
{
    void   *malloc_call = NULL;
    C8_App_ptr app = NULL;

    malloc_call = malloc(sizeof(*app));
    assert(malloc_call);
    app = (C8_App_ptr) malloc_call;

    // init, reset and load program
    app->update_freq = 1.0f / fps;
    app->rng = C8_Random_new(42); // todo: init from time
    app->disp = C8_Display_init();
    app->keys = C8_Keyboard_init();
    app->c8 = C8_init(app->rng, app->disp, app->keys);
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
    C8_Keyboard_free(&(app->keys));

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

        process_events(&quit);
        process_keyboard(app->keys);

        timer_accum  += delta;
        while (timer_accum  > TIMER_FREQ) {
            timer_accum -= TIMER_FREQ;
            C8_timers(app->c8);
        }

        update_accum += delta;
        while (update_accum > app->update_freq) {
            update_accum -= app->update_freq;
            C8_cycle(app->c8);
        }
        
        render_c8_disp(app->renderer, app->disp);
    }
}
