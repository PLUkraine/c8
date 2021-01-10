#include "logging.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void log_info(char *format, ...)
{
    va_list args;
   
    printf("INFO: ");
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

void log_error(char *format, ...)
{
    va_list args;
   
    printf("ERROR: ");
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}