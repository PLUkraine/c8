#include "logging.h"

#include <SDL2/SDL_log.h>

void log_info(char *format, ...)
{
    va_list args;
   
    va_start(args, format);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);
    va_end(args);
}

void log_error(char *format, ...)
{
    va_list args;
   
    va_start(args, format);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, format, args);
    va_end(args);
}