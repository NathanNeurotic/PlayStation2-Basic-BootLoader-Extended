#ifndef UTIL_SAFE_H
#define UTIL_SAFE_H

#include "util_safe_compat.h"

/**
 * @brief strlcpy-style copy that guarantees NUL termination.
 * @param dst destination buffer
 * @param src source string (may be NULL)
 * @param dst_size total size of the destination buffer
 * @return length of the source string (0 if src is NULL)
 */
size_t util_strlcpy(char *dst, const char *src, size_t dst_size);

/**
 * @brief memcpy-style copy with bounds checking.
 * @param dst destination buffer
 * @param dst_size total size of destination buffer
 * @param src source buffer
 * @param src_size bytes to copy from source
 * @return 0 on success, negative value on invalid arguments or truncation
 */
int util_memcpy_s(void *dst, size_t dst_size, const void *src, size_t src_size);

/**
 * @brief snprintf wrapper that guarantees NUL termination even on error.
 * @param dst destination buffer
 * @param dst_size total size of the destination buffer
 * @param fmt format string
 * @return number of characters (excluding the NUL) that would have been written, or a negative value on error
 */
int util_snprintf(char *dst, size_t dst_size, const char *fmt, ...);

/**
 * @brief bounded strlen equivalent that stops after max_len characters.
 * @param str input string (may be NULL)
 * @param max_len maximum number of characters to inspect
 * @return number of characters inspected, capped at max_len
 */
size_t util_bounded_strnlen(const char *str, size_t max_len);

#endif
