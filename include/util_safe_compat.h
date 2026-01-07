#ifndef UTIL_SAFE_COMPAT_H
#define UTIL_SAFE_COMPAT_H

#include "platform_includes.h"

#ifndef SIZE_MAX
typedef unsigned long size_t;
#define SIZE_MAX ((size_t)-1)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef va_list
typedef void *va_list;
#endif

/* Minimal stubs to satisfy static analysis when libc prototypes are unavailable. */
#ifndef MEMMOVE_DECLARED
#define MEMMOVE_DECLARED 1
void *memmove(void *dest, const void *src, size_t n);
#endif

#ifndef VSNPRINTF_DECLARED
#define VSNPRINTF_DECLARED 1
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
#endif

#endif /* UTIL_SAFE_COMPAT_H */
