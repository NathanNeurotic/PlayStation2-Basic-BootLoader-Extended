#ifdef __CPPCHECK__
#include "cppcheck_stubs/kernel.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <kernel.h>
#endif
#ifdef __CPPCHECK__
#include "cppcheck_stubs/sifrpc.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <sifrpc.h>
#endif
#ifdef __CPPCHECK__
#include "cppcheck_stubs/libcdvd.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <libcdvd.h>
#endif
#ifdef __CPPCHECK__
#include "cppcheck_stubs/string.h"
#else
// cppcheck-suppress missingIncludeSystem
#include <string.h>
#endif
// cppcheck-suppress missingIncludeSystem
#include <stdio.h>

#include "debugprintf.h"
#include "libcdvd_add.h"
#include "util_safe.h"

static unsigned char MECHACON_CMD_S36_supported = 0, MECHACON_CMD_S27_supported = 0, MECHACON_CMD_S24_supported = 0;
enum {
    REGION_DATA_LEN = 13,
    PS1_BOOT_PARAM_LEN = 11
};

int cdInitAdd(void)
{
    int result, i;
    u32 status;
    u8 MECHA_version_data[3];
    unsigned int MECHA_version;

    // Like how CDVDMAN checks sceCdMV(), do not continuously attempt to get the MECHACON version because some consoles (e.g. DTL-H301xx) can't return one.
    for (i = 0; i <= 100; i++) {
        if ((result = sceCdMV(MECHA_version_data, &status)) != 0 && ((status & 0x80) == 0)) {
            MECHA_version = MECHA_version_data[2] | ((unsigned int)MECHA_version_data[1] << 8) | ((unsigned int)MECHA_version_data[0] << 16);
            MECHACON_CMD_S36_supported = (0x5FFFF < MECHA_version); // v6.0 and later
            MECHACON_CMD_S27_supported = (0x501FF < MECHA_version); // v5.2 and later
            MECHACON_CMD_S24_supported = (0x4FFFF < MECHA_version); // v5.0 and later
            return 0;
        }
    }

    DPRINTF("Failed to get MECHACON version: %d 0x%x\n", result, status);

    return -1;
}

int custom_sceCdReadRegionParams(u8 *data, size_t data_size, u32 *stat)
{
    unsigned char RegionData[15];
    int result;

    if (data == NULL || stat == NULL)
        return 0;

    memset(data, 0, data_size);
    if (data_size < REGION_DATA_LEN) {
        *stat = 0x100;
        return 0;
    }

    if (MECHACON_CMD_S36_supported) {
        // if ((result = sceCdApplySCmd(0x36, NULL, 0, RegionData, sizeof(RegionData))) != 0)
        if ((result = sceCdApplySCmd(0x36, NULL, 0, RegionData)) != 0) {
            *stat = RegionData[0];
            // Validate destination size to prevent CWE-120 when copying fixed region bytes.
            if (util_memcpy_s(data, data_size, &RegionData[1], REGION_DATA_LEN) != 0) {
                result = 0;
            }
        }
    } else {
        *stat = 0x100;
        result = 1;
    }

    return result;
}

int sceCdBootCertify(const u8 *data)
{
    int result;
    unsigned char CmdResult;

    // if ((result = sceCdApplySCmd(0x1A, data, 4, &CmdResult, 1)) != 0)
    if ((result = sceCdApplySCmd(0x1A, data, 4, &CmdResult)) != 0) {
        result = CmdResult;
    }

    return result;
}

int sceCdRM(char *ModelName, u32 *stat)
{
    unsigned char rdata[9];
    unsigned char sdata;
    int result1, result2;
    const size_t modelNameSize = 16; // Two 8-byte chunks

    if (ModelName == NULL || stat == NULL) {
        return 0;
    }

    memset(rdata, 0, sizeof(rdata));
    memset(ModelName, 0, modelNameSize);

    sdata = 0;
    result1 = sceCdApplySCmd(0x17, &sdata, 1, rdata);
    // result1 = sceCdApplySCmd(0x17, &sdata, 1, rdata, 9);

    *stat = rdata[0];
    if (result1 != 0 && util_memcpy_s(ModelName, modelNameSize, &rdata[1], 8) != 0) {
        result1 = 0;
    }

    sdata = 8;
    result2 = sceCdApplySCmd(0x17, &sdata, 1, rdata);
    // result2 = sceCdApplySCmd(0x17, &sdata, 1, rdata, 9);

    *stat |= rdata[0];
    if (result2 != 0 && util_memcpy_s(&ModelName[8], modelNameSize - 8, &rdata[1], 8) != 0) {
        result2 = 0;
    }

    return ((result1 != 0 && result2 != 0) ? 1 : 0);
}

/*
    This function provides an equivalent of the sceCdReadPS1BootParam function from the newer CDVDMAN modules. The old CDVDFSV and CDVDMAN modules don't support this S-command.
    It's supported by only slimline consoles, and returns the boot path for the inserted PlayStation disc.
*/
int custom_sceCdReadPS1BootParam(char *param, size_t param_size, u32 *stat)
{
    u8 out[16];
    int result;

    if (param == NULL || stat == NULL)
        return 0;

    memset(param, 0, param_size);
    if (param_size < PS1_BOOT_PARAM_LEN) {
        *stat = 0x100;
        return 0;
    }

    if (MECHACON_CMD_S27_supported) {
        // if ((result = sceCdApplySCmd(0x27, NULL, 0, out, 13)) != 0)
        if ((result = sceCdApplySCmd(0x27, NULL, 0, out)) != 0) {
            *stat = out[0];
            // Validate destination size to prevent CWE-120 while copying fixed boot bytes.
            if (util_memcpy_s(param, param_size, &out[1], PS1_BOOT_PARAM_LEN) != 0) {
                result = 0;
            }
        }
    } else {
        *stat = 0x100;
        result = 1;
    }

    return result;
}

int sceCdRcBypassCtl(int bypass, u32 *stat)
{ // TODO: not implemented.
    u8 in[16], out[16];
    int result;

    memset(in, 0, 11);
    if (MECHACON_CMD_S24_supported) {
        // TODO
        // if ((result = sceCdApplySCmd(0x24, &bypass, 4, out, 13)) != 0)
        if ((result = sceCdApplySCmd(0x24, &bypass, 4, out)) != 0) {
            *stat = out[0];
        }
    } else {
        *stat = 0x100;
        result = 1;
    }

    return result;
}
