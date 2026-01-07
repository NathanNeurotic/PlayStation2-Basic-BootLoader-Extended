#ifndef UTIL_SAFE_COMPAT_H
#define UTIL_SAFE_COMPAT_H

#if defined(__has_include)
  #if __has_include(<stddef.h>)
    #include <stddef.h>
  #endif
  #if __has_include(<stdarg.h>)
    #include <stdarg.h>
  #endif
  #if __has_include(<stdint.h>)
    #include <stdint.h>
  #endif
  #if __has_include(<limits.h>)
    #include <limits.h>
  #endif
  #if __has_include(<string.h>)
    #include <string.h>
  #endif
  #if __has_include(<stdio.h>)
    #include <stdio.h>
  #endif
#endif

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
