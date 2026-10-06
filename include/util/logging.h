#pragma once

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

void log_core_engine(LogLevel level, const char* file, int line, int errnum, const char* fmt, ...);

#define LOG_INFO(...)  log_core_engine(LOG_LEVEL_INFO,  __FILE_NAME__, __LINE__, 0,     __VA_ARGS__)
#define LOG_WARN(...)  log_core_engine(LOG_LEVEL_WARN,  __FILE_NAME__, __LINE__, 0,     __VA_ARGS__)
#define LOG_ERROR(...) log_core_engine(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, 0,     __VA_ARGS__)

#define LOG_SYS_ERROR(...) log_core_engine(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, errno, __VA_ARGS__)

#ifdef DEBUG
#define LOG_DEBUG(...) log_core_engine(LOG_LEVEL_DEBUG, __FILE_NAME__, __LINE__, 0, __VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#endif