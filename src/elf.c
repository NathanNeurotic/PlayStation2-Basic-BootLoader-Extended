// cppcheck-suppress missingIncludeSystem
#include <stdio.h>
// cppcheck-suppress missingIncludeSystem
#include <fcntl.h>
// cppcheck-suppress missingIncludeSystem
#include <unistd.h>
#ifdef __CPPCHECK__
#include "cppcheck_stubs/tamtypes.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <tamtypes.h>
#endif
#ifdef __CPPCHECK__
#include "cppcheck_stubs/kernel.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <kernel.h>
#endif
#ifdef __CPPCHECK__
#include "cppcheck_stubs/elf-loader.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <elf-loader.h>
#endif
#include "debugprintf.h"
#define MAX_PATH 1025
#ifdef DEBUG
#define DBGWAIT(T) sleep(T)
#else
#define DBGWAIT(T)
#endif


void RunLoaderElf(const char *filename, const char *party)
{
    DPRINTF("%s\n", __FUNCTION__);
    if (party == NULL) {
        DPRINTF("LoadELFFromFile(%s, 0, NULL)\n", filename);
        DBGWAIT(2);
        LoadELFFromFile(filename, 0, NULL);
    } else {
        DPRINTF("LoadELFFromFileWithPartition(%s, %s, 0, NULL);\n", filename, party);
        DBGWAIT(2);
        LoadELFFromFileWithPartition(filename, party, 0, NULL);
    }
}
