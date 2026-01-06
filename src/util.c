#include <errno.h>
#include "util_safe_compat.h"
#ifndef __CPPCHECK__
#include <limits.h>
#endif
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <tamtypes.h>
#include <loadfile.h>
#include <malloc.h>
#if defined(HDD) || defined(HDD_RUNTIME)
#include <assert.h>
#endif

#include "util.h"
#include "util_safe.h"

int exist(const char *filepath)
{
    if (filepath == NULL)
        return 0;

    const int fdn = open(filepath, O_RDONLY);
    if (fdn < 0)
        return 0;

    close(fdn);

    return 1;
}

void delay(int count)
{
    int i;
    for (i = 0; i < count; i++) {
        int ret;

        ret = 0x01000000;
        while (ret--)
            asm("nop\nnop\nnop\nnop");
    }
}


int get_CNF_string(char **CNF_p_p,
                   char **name_p_p,
                   char **value_p_p)
{
    char *np, *vp, *tp = *CNF_p_p;

start_line:
    while ((*tp <= ' ') && (*tp > '\0'))
        tp += 1; // Skip leading whitespace, if any
    if (*tp == '\0')
        return 0;  // but exit at EOF
    np = tp;       // Current pos is potential name
    if (*tp < 'A') // but may be a comment line
    {              // We must skip a comment line
        while ((*tp != '\r') && (*tp != '\n') && (*tp > '\0'))
            tp += 1;     // Seek line end
        goto start_line; // Go back to try next line
    }

    while ((*tp >= 'A') || ((*tp >= '0') && (*tp <= '9')))
        tp += 1; // Seek name end
    if (*tp == '\0')
        return 0; // but exit at EOF

    while ((*tp <= ' ') && (*tp > '\0'))
        *tp++ = '\0'; // zero&skip post-name whitespace
    if (*tp != '=')
        return 0; // exit (syntax error) if '=' missing
    *tp++ = '\0'; // zero '=' (possibly terminating name)

    while ((*tp <= ' ') && (*tp > '\0')      // Skip pre-value whitespace, if any
           && (*tp != '\r') && (*tp != '\n') // but do not pass the end of the line
           && (*tp != '\7')                  // allow ctrl-G (BEL) in value
    )
        tp += 1;
    if (*tp == '\0')
        return 0; // but exit at EOF
    vp = tp;      // Current pos is potential value

    while ((*tp != '\r') && (*tp != '\n') && (*tp != '\0'))
        tp += 1; // Seek line end
    if (*tp != '\0')
        *tp++ = '\0'; // terminate value (passing if not EOF)
    while ((*tp <= ' ') && (*tp > '\0'))
        tp += 1; // Skip following whitespace, if any

    *CNF_p_p = tp;   // return new CNF file position
    *name_p_p = np;  // return found variable name
    *value_p_p = vp; // return found variable value
    return 1;        // return control to caller
} // Ends get_CNF_string

#if defined(HDD) || defined(HDD_RUNTIME)
//By fjtrujy
char **str_split(char *a_str, const char a_delim)
{
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + util_bounded_strnlen(a_str, SIZE_MAX) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result) {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            char *copy = strdup(token);

            if (copy == NULL) {
                for (size_t cleanup_idx = 0; cleanup_idx < idx; cleanup_idx++) {
                    free(*(result + cleanup_idx));
                    *(result + cleanup_idx) = NULL;
                }
                free(result);
                result = NULL;
                return NULL;
            }

            *(result + idx++) = copy;
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

/**
 * @brief  method returns 0 if it can extract needed info from path, otherwise a negative error code.
 * In case of success, it also updates mountString, mountPoint and newCWD parameters
 * It splits path by ":", and requires a minimum of 3 elements
 * Example: if path = hdd0:__common:pfs:/retroarch/ then
 * mountString = "pfs:"
 * mountPoint = "hdd0:__common"
 * newCWD = pfs:/retroarch/
 * return 0 on success, negative error code on failure
*/
int getMountInfo(char *path, char *mountString, size_t mountStringSize, char *mountPoint, size_t mountPointSize, char *newCWD, size_t newCWDSize)
{
    if (path == NULL)
        return -EINVAL;

    int i = 0;
    char **tokens = str_split(path, ':');
    int ret = -EINVAL;

    if (!tokens) {
        ret = -ENOMEM;
        goto cleanup;
    }

    for (i = 0; *(tokens + i); i++)
        ;

    if (i < 3)
        goto cleanup;

    if (mountPoint != NULL) {
        if (mountPointSize == 0) {
            ret = -ENAMETOOLONG;
            goto cleanup;
        }
        int written = snprintf(mountPoint, mountPointSize, "%s:%s", tokens[0], tokens[1]);
        if (written < 0) {
            ret = -EIO;
            goto cleanup;
        }
        if ((size_t)written >= mountPointSize) {
            ret = -ENAMETOOLONG;
            goto cleanup;
        }
    }

    if (mountString != NULL) {
        if (mountStringSize == 0) {
            ret = -ENAMETOOLONG;
            goto cleanup;
        }
        int written = snprintf(mountString, mountStringSize, "%s:", tokens[2]);
        if (written < 0) {
            ret = -EIO;
            goto cleanup;
        }
        if ((size_t)written >= mountStringSize) {
            ret = -ENAMETOOLONG;
            goto cleanup;
        }
    }

    if (newCWD != NULL) {
        if (newCWDSize == 0) {
            ret = -ENAMETOOLONG;
            goto cleanup;
        }
        int written = snprintf(newCWD, newCWDSize, "%s:%s", tokens[2], i > 3 ? tokens[3] : "");
        if (written < 0) {
            ret = -EIO;
            goto cleanup;
        }
        if ((size_t)written >= newCWDSize) {
            ret = -ENAMETOOLONG;
            goto cleanup;
        }
    }

    ret = 0;

cleanup:
    if (tokens) {
        for (int idx = 0; *(tokens + idx); idx++) {
            free(*(tokens + idx));
            *(tokens + idx) = NULL;
        }
        free(tokens);
        tokens = NULL;
    }

    return ret;
}
#endif
