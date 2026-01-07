#include "util_safeio.h"

#include <unistd.h>

/* Centralized bounded read helpers to address Codacy CWE-120/CWE-20 findings. */

ssize_t safe_read_once_nt(int fd, char *buf, size_t buf_sz)
{
    if (buf == NULL || buf_sz == 0) {
        return -1;
    }

    ssize_t r = read(fd, buf, buf_sz - 1);
    if (r > 0) {
        buf[r] = '\0';
    } else {
        buf[0] = '\0';
    }

    return r;
}

ssize_t safe_read_fully(int fd, void *buf, size_t buf_sz)
{
    if (buf == NULL || buf_sz == 0) {
        return -1;
    }

    size_t total = 0;
    while (total < buf_sz) {
        ssize_t r = read(fd, (unsigned char *)buf + total, buf_sz - total);
        if (r > 0) {
            total += (size_t)r;
        } else if (r == 0) {
            break;
        } else {
            return (total > 0) ? (ssize_t)total : -1;
        }
    }

    return (ssize_t)total;
}

ssize_t safe_read_fully_nt(int fd, char *buf, size_t buf_sz)
{
    if (buf == NULL || buf_sz == 0) {
        return -1;
    }

    ssize_t total = safe_read_fully(fd, buf, buf_sz - 1);
    if (total >= 0) {
        buf[total] = '\0';
    } else {
        buf[0] = '\0';
    }

    return total;
}
