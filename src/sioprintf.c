#ifdef __CPPCHECK__
#include "cppcheck_stubs/stdarg.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <stdarg.h>
#endif
// cppcheck-suppress missingIncludeSystem
#include <stdio.h>
#ifdef __CPPCHECK__
#include "cppcheck_stubs/sio.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <sio.h>
#endif
void sio_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char str[256];
    vsnprintf(str, sizeof(str), fmt, args);
    sio_putsn(str);
    va_end(args);
}
