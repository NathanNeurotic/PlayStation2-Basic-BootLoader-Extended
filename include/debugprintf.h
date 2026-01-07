#ifndef DEBUG_PRINTF
#define DEBUG_PRINTF

#include <stdarg.h>
#include <stdio.h>

#ifdef SCR_PRINT
#include <debug.h>
#endif

#define DEBUGPRINTF_BUFFER_SIZE 256

#ifdef EE_SIO_DEBUG
void sio_printf(const char *fmt, ...);
#include <SIOCookie.h>
#define DPRINTF_INIT() ee_sio_start(38400, 0, 0, 0, 0, 1);
static inline void debugprintf(const char *fmt, ...)
{
    char buffer[DEBUGPRINTF_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    sio_printf("%s", buffer);
}
#elif SCR_PRINT
#define DPRINTF_INIT()
static inline void debugprintf(const char *fmt, ...)
{
    char buffer[DEBUGPRINTF_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    scr_printf("%s", buffer);
}
#elif COMMON_PRINTF
#define DPRINTF_INIT()
static inline void debug_vprintf_safe(const char *fmt, va_list args)
{
    if (fmt == NULL) {
        return;
    }

#if defined(__GNUC__)
    if (!__builtin_constant_p(fmt)) {
        /* CWE-134 guard: only literal formats are interpreted. */
        printf("%s", fmt);
        return;
    }
#else
    /* CWE-134 guard: only literal formats are interpreted. */
    printf("%s", fmt);
    return;
#endif

    {
        char buffer[DEBUGPRINTF_BUFFER_SIZE];
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        printf("%s", buffer);
    }
}

static inline void debugprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    debug_vprintf_safe(fmt, args);
    va_end(args);
}
#else
#define DPRINTF_INIT()
static inline void debugprintf(const char *fmt, ...)
{
    (void)fmt;
}
#define NO_DPRINTF
#endif

#ifdef NO_DPRINTF
#define DPRINTF(...) ((void)0)
#define DPRINTS(s)   ((void)0)
#else
#define DPRINTF(fmt, ...) debugprintf("" fmt "", ##__VA_ARGS__)
#define DPRINTS(s)        debugprintf("%s", (s))
#endif

#endif //DEBUG_PRINTF
