#ifndef UTIL_SAFEIO_H
#define UTIL_SAFEIO_H

#include "util_safe_compat.h"

#if defined(__has_include)
  #if __has_include(<unistd.h>)
    #include <unistd.h>
  #endif
#endif

/*
 * Safe read helpers to satisfy Codacy CWE-120/CWE-20 guidance:
 * ensure bounded reads, consistent error handling, and optional NUL termination.
 */

/**
 * @brief Read at most buf_sz-1 bytes and always NUL-terminate on success or error.
 * @param fd file descriptor
 * @param buf destination buffer
 * @param buf_sz size of destination buffer
 * @return bytes read (>=0), or negative on error
 */
ssize_t safe_read_once_nt(int fd, char *buf, size_t buf_sz);

/**
 * @brief Read until EOF or buffer full without exceeding buf_sz.
 * @param fd file descriptor
 * @param buf destination buffer
 * @param buf_sz size of destination buffer
 * @return total bytes read, or negative on error before any bytes were read
 */
ssize_t safe_read_fully(int fd, void *buf, size_t buf_sz);

/**
 * @brief Read until EOF or buffer full with guaranteed NUL termination.
 * @param fd file descriptor
 * @param buf destination buffer
 * @param buf_sz size of destination buffer
 * @return total bytes read, or negative on error before any bytes were read
 */
ssize_t safe_read_fully_nt(int fd, char *buf, size_t buf_sz);

/**
 * @brief size_t min helper for bounded operations.
 */
#define safe_min_size(a, b) ((size_t)((a) < (b) ? (a) : (b)))

#endif /* UTIL_SAFEIO_H */
