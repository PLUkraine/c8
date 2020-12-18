#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "cummon.h"
#include "logging.h"


int main(int argc, char** argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    SDL_Quit();
    return 0;
}
