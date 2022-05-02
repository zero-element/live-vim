
enum
{
    LOGLEVEL_NONE,
    LOGLEVEL_ERROR,
    LOGLEVEL_WARN,
    LOGLEVEL_INFO,
    LOGLEVEL_DEBUG
};

#define LOG_LEVEL LOGLEVEL_INFO
#ifdef LOG_LEVEL
#include <stdio.h>
#define LOG_PRINT(level, print_this...)  \
    {                                    \
        if (level <= LOG_LEVEL)          \
        {                                \
            fprintf(stderr, print_this); \
            fflush(stderr);              \
        }                                \
    }
#else
#define LOG_PRINT(level, print_this...)
#endif

#define LOG_ERROR(print_this...) LOG_PRINT(LOGLEVEL_ERROR, "ERROR: " print_this)
#define LOG_WARN(print_this...) LOG_PRINT(LOGLEVEL_WARN, "WARN: " print_this)
#define LOG_INFO(print_this...) LOG_PRINT(LOGLEVEL_INFO, "INFO: " print_this)
#define LOG_DEBUG(print_this...) LOG_PRINT(LOGLEVEL_DEBUG, "DEBUG: " print_this)

