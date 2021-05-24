#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "common.h"
#include "logging.h"
#include "c8_app.h"

#define DEFAULT_FPS 100


size_t parse_fps(const char *fps_str)
{
    size_t fps = strtol(fps_str, (char **)NULL, 10);
    return fps > 0 ? fps : DEFAULT_FPS;
}


int main(int argc, char** argv)
{
    if (argc < 3)
    {
        log_error("Usage: %s <fps> <path-to-chip8-game>\n", argv[0]);
        exit(1);
    }

    SDL_Init(SDL_INIT_EVERYTHING);
    C8_App_ptr c8_app = C8_App_init(argv[2], parse_fps(argv[1]));

    C8_App_main_loop(c8_app);

    C8_App_free(&c8_app);
    SDL_Quit();
    return 0;
}
