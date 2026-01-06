#include "util_safe.h"
#include "util_safe_compat.h"

size_t util_bounded_strnlen(const char *str, size_t max_len)
{
    size_t len = 0;

    if (str == NULL)
        return 0;

    while (len < max_len && str[len] != '\0') {
        len++;
    }

    return len;
}

int util_memcpy_s(void *dst, size_t dst_size, const void *src, size_t src_size)
{
    if (dst == NULL || src == NULL)
        return -1;
    if (src_size == 0)
        return 0;
    if (dst_size < src_size)
        return -1;

    memmove(dst, src, src_size);
    return 0;
}

size_t util_strlcpy(char *dst, const char *src, size_t dst_size)
{
    const size_t src_len = util_bounded_strnlen(src, SIZE_MAX);

    if (dst_size > 0 && dst != NULL) {
        const size_t copy_len = (src_len >= dst_size) ? dst_size - 1 : src_len;

        if (src != NULL && copy_len > 0) {
            if (util_memcpy_s(dst, dst_size, src, copy_len) != 0)
                return src_len;
        }
        dst[copy_len] = '\0';
    }

    return src_len;
}

int util_snprintf(char *dst, size_t dst_size, const char *fmt, ...)
{
    va_list args;
    int result;

    va_start(args, fmt);
    result = vsnprintf(dst, dst_size, fmt, args);
    va_end(args);

    if (dst_size > 0) {
        if (result < 0 || (size_t)result >= dst_size)
            dst[dst_size - 1] = '\0';
    }

    return result;
}
