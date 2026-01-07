#include "util_safeio.h"

#include <unistd.h>

/* Centralized bounded read helpers to address Codacy CWE-120/CWE-20 findings. */

static ssize_t safe_read_loop(int fd, char *buf, size_t buf_sz, int nul_terminate)
{
    if (buf == NULL || buf_sz == 0) {
        return -1;
    }

    size_t offset = 0;
    while (offset < buf_sz) {
        size_t max_read = buf_sz - offset;

        if (nul_terminate) {
            if (max_read <= 1) {
                break; // Leave space for the terminator.
            }
            max_read -= 1;
        }

        ssize_t r = read(fd, buf + offset, max_read);
        if (r <= 0) {
            break;
        }

        offset += (size_t)r;
    }

    if (nul_terminate) {
        buf[offset] = '\0';
    }

    return (ssize_t)offset;
}

ssize_t safe_read_once_nt(int fd, char *buf, size_t buf_sz)
{
    return safe_read_loop(fd, buf, buf_sz, 1);
}

ssize_t safe_read_fully_bin(int fd, void *buf, size_t buf_sz)
{
    return safe_read_loop(fd, (char *)buf, buf_sz, 0);
}

ssize_t safe_read_fully_nt(int fd, char *buf, size_t buf_sz)
{
    return safe_read_loop(fd, buf, buf_sz, 1);
}
