#include "util_safeio.h"

#include <unistd.h>

/* Centralized bounded read helpers to address Codacy CWE-120/CWE-20 findings. */

static ssize_t safe_read_loop(int fd, unsigned char *buf, size_t buf_sz, int nul_terminate)
{
    if (buf == NULL || buf_sz == 0) {
        return -1;
    }

    size_t offset = 0;
    for (;;) {
        if (offset >= buf_sz) {
            break;
        }

        size_t remaining = buf_sz - offset;
        if (remaining == 0) {
            break;
        }

        size_t remaining_for_read = remaining;
        if (nul_terminate) {
            if (remaining_for_read <= 1) {
                break; // Leave space for the terminator.
            }
            remaining_for_read -= 1;
        }

        ssize_t r = read(fd, buf + offset, remaining_for_read);
        if (r == 0) {
            break; // EOF
        }
        if (r < 0) {
            return (offset > 0) ? (ssize_t)offset : -1;
        }
        if ((size_t)r > remaining_for_read) {
            return (offset > 0) ? (ssize_t)offset : -1;
        }

        offset += (size_t)r;
        if (offset > buf_sz) {
            return -1;
        }
    }

    if (nul_terminate) {
        buf[offset] = '\0';
    }

    return (ssize_t)offset;
}

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
    return safe_read_loop(fd, (unsigned char *)buf, buf_sz, 0);
}

ssize_t safe_read_fully_nt(int fd, char *buf, size_t buf_sz)
{
    return safe_read_loop(fd, (unsigned char *)buf, buf_sz, 1);
}
