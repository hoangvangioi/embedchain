#include "logger.h"

static FILE           *logfile   = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void set_log_file(const char *filename)
{
    if (logfile != NULL)
    {
        fclose(logfile);
    }
    logfile = fopen(filename, "a");
    if (logfile == NULL)
    {
        perror("Error opening log file");
    }
}

void close_logger()
{
    if (logfile != NULL)
    {
        fclose(logfile);
        logfile = NULL;
    }
}

void log_message(LogLevel level, const char *message)
{
    if (logfile == NULL)
    {
        fprintf(stderr, "Logfile not opened. Use set_log_file() to open a logfile.\n");
        return;
    }

    pthread_mutex_lock(&log_mutex);

    /* Lấy thời gian hiện tại */
    time_t     now     = time(NULL);
    struct tm *tm_info = localtime(&now);
    char       time_str[26];
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    /* Chọn tiền tố dựa trên cấp độ log */
    const char *level_str;
    switch (level)
    {
        case LOG_INFO:
            level_str = "INFO";
            break;
        case LOG_WARN:
            level_str = "WARN";
            break;
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        case LOG_DEBUG:
            level_str = "DEBUG";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }

    /* Ghi log vào file */
    fprintf(logfile, "[%s] [%s] %s\n", time_str, level_str, message);
    fflush(logfile); /* Đảm bảo ghi ngay lập tức */

    pthread_mutex_unlock(&log_mutex);
}
