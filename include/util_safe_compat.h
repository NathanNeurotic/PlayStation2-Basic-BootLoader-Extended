#ifndef UTIL_SAFE_COMPAT_H
#define UTIL_SAFE_COMPAT_H

/*
 * Compatibility layer for environments where cppcheck/Codacy cannot locate
 * system headers. Real builds will still include the normal headers.
 */
#ifndef __CPPCHECK__
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#else
/* Minimal stand-ins so static analysis can proceed without missing includes. */
typedef unsigned long size_t;
typedef signed long ptrdiff_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long uintptr_t;
typedef void *va_list;
#ifndef NULL
#define NULL ((void *)0)
#endif
#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)-1)
#endif
/* Forward declarations used by util_safe.c */
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
int snprintf(char *str, size_t size, const char *format, ...);
void *memmove(void *dest, const void *src, size_t n);
#endif

#endif /* UTIL_SAFE_COMPAT_H */
