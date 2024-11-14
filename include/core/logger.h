#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>
#include <stdio.h>
#include <time.h>

typedef enum
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG
} LogLevel;

#ifdef __cplusplus
extern "C"
{
#endif

    void log_message(LogLevel level, const char *message);
    void set_log_file(const char *filename);
    void close_logger();

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_H */
