#ifndef LOGGING_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void log_info(char *format, ...);
void log_error(char *format, ...);

#ifdef __cplusplus
}
#endif

#endif