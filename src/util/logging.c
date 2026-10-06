#include "util/logging.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

void log_core_engine(const LogLevel level, const char* file, const int line, int errnum, const char* fmt, ...) {
    FILE* stream = level == LOG_LEVEL_ERROR || level == LOG_LEVEL_WARN ? stderr : stdout;

    const time_t now = time(NULL);
    const struct tm *t = localtime(&now);
    char time_buf[12];
    strftime(time_buf, sizeof(time_buf), "%H:%M:%S", t);

    const char* level_strings[] = {"DEBUG", "INFO", "WARN", "ERROR"};

    fprintf(stream, "[%s] [%s] (%s:%d): ", time_buf, level_strings[level], file, line);

    if (errnum != 0) {
        char err_desc[256];
#if defined(_WIN32) || defined(_WIN64)
        strerror_s(err_desc, sizeof(err_desc), errnum);
#elif defined(_GNU_SOURCE)
        char* gnu_msg = strerror_r(errnum, err_desc, sizeof(err_desc));
        snprintf(err_desc, sizeof(err_desc), "%s", gnu_msg);
#else
        if (strerror_r(errnum, err_desc, sizeof(err_desc)) != 0) {
            snprintf(err_desc, sizeof(err_desc), "Unknown error");
        }
#endif
        fprintf(stream, "[System Error: %s (%d)] ", err_desc, errnum);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stream, fmt, args);
    va_end(args);

    fprintf(stream, "\n");

    if (stream == stdout) {
        fflush(stdout);
    }
}
