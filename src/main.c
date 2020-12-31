#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "cummon.h"
#include "logging.h"
#include "c8_app.h"


int main(int argc, char** argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    
    SDL_Init(SDL_INIT_EVERYTHING);
    C8_App_ptr c8_app = C8_App_init();

    C8_App_main_loop(c8_app);

    C8_App_free(&c8_app);
    SDL_Quit();
    return 0;
}
