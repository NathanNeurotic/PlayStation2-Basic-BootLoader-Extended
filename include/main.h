#ifndef MAIN_H
#define MAIN_H
#define NEWLIB_PORT_AWARE

#include <stdlib.h>
#include <stdio.h>
#ifdef __CPPCHECK__
#include "cppcheck_stubs/string.h"
#else
#include <string.h>
#endif
#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>

#ifdef __CPPCHECK__
#include "cppcheck_shims.h"
#include "cppcheck_stubs/tamtypes.h"
#include "cppcheck_stubs/kernel.h"
#include "cppcheck_stubs/sifrpc.h"
#include "cppcheck_stubs/loadfile.h"
#include "cppcheck_stubs/debug.h"
#include "cppcheck_stubs/iopcontrol.h"
#include "cppcheck_stubs/iopheap.h"
#include "cppcheck_stubs/sbv_patches.h"
#include "cppcheck_stubs/ps2sdkapi.h"
#include "cppcheck_stubs/usbhdfsd-common.h"
#include "cppcheck_stubs/osd_config.h"
#include "cppcheck_stubs/libpad.h"
#include "cppcheck_stubs/libmc.h"
#include "cppcheck_stubs/libcdvd.h"
#else
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <debug.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <sbv_patches.h>
#include <ps2sdkapi.h>
#include <usbhdfsd-common.h>
#include <osd_config.h>
#include <libpad.h>
#include <libmc.h>
#include <libcdvd.h>
#endif

#include "debugprintf.h"
#include "pad.h"
#include "util.h"
#include "util_safe.h"
#include "common.h"

#include "libcdvd_add.h"
#include "dvdplayer.h"
#include "OSDInit.h"
#include "OSDConfig.h"
#include "OSDHistory.h"
#include "ps1.h"
#include "ps2.h"
#include "modelname.h"
#include "banner.h"

#ifdef PSX
#ifdef __CPPCHECK__
#include "cppcheck_stubs/iopcontrol_special.h"
#else
#include <iopcontrol_special.h>
#endif
#include "psx/plibcdvd_add.h"
#endif

#ifdef DEV9
extern int dev9_loaded;
int loadDEV9(void);
#endif

// For avoiding define NEWLIB_AWARE
void fioInit();
#define RBG2INT(R, G, B) ((0 << 24) + (R << 16) + (G << 8) + B)
#include "irx_import.h"

void RunLoaderElf(const char *filename, const char *party);
void EMERGENCY(void);
// Reboot the PS1 CPU and perform additional tasks if building for PSX DESR
void ResetIOP(void);
void SetDefaultSettings(void);
void TimerInit(void);
u64 Timer(void);
void TimerEnd(void);

/// check path for processing pseudo-devices like `mc?:/`
char *CheckPath(char *path);
static void AlarmCallback(s32 alarm_id, u16 time, void *common);
int dischandler();
// there is no need to call this on a PSX DESR since OSDSYS performs it at boot
void CDVDBootCertify(u8 romver[16]);
void credits(void);
void CleanUp(void);
int LoadUSBIRX(void);
// Execute OSDSYS with parameters to avoid booting memory card or HDD updates
void runOSDNoUpdate(void);
#ifdef PSX
// Satisfy special necesities of the PSX DESR
static void InitPSX();
#endif
#ifndef NO_TEMP_DISP
/// @brief Print console temperature on screen
/// @note only supported by DRAGON Mechacons. function will do nothing on older mechacon
void PrintTemperature();
#endif
#if defined(HDD) || defined(HDD_RUNTIME)
int LoadHDDIRX(void);             // Load HDD IRXes
int LoadFIO(void);                // Load FileXio and it´s dependencies
int MountParty(const char *path); ///processes strings in the format `hdd0:/$PARTITION:pfs:$PATH_TO_FILE/` to mount partition
int mnt(const char *path);        ///mount partition specified on path
#endif

#ifdef UDPTTY
void loadUDPTTY();
#endif

#if defined(HDD) || defined(HDD_RUNTIME)
#ifdef __CPPCHECK__
#include "cppcheck_stubs/hdd-ioctl.h"
#include "cppcheck_stubs/io_common.h"
#include "cppcheck_stubs/libpwroff.h"
#else
#include <hdd-ioctl.h>
#include <io_common.h>
#include <libpwroff.h>
#endif
#include <assert.h>
extern char PART[128];
extern int HDD_USABLE;
#define MPART PART
int LoadHDDIRX(void);             // Load HDD IRXes
int MountParty(const char *path); ///processes strings in the format `hdd0:/$PARTITION:pfs:$PATH_TO_FILE/` to mount partition
int mnt(const char *path);        ///mount partition specified on path
void HDDChecker();
void poweroffCallback(void *arg);
#else //this ensures that when HDD support is not available, loaded ELFs dont have any extra arg...
#define MPART NULL
#endif

#if defined(MX4SIO) || defined(MX4SIO_RUNTIME)
int LookForBDMDevice(void);
#endif

#ifdef FILEXIO
#ifdef __CPPCHECK__
#include "cppcheck_stubs/fileXio_rpc.h"
#else
#include <fileXio_rpc.h>
#endif
int LoadFIO(void); // Load FileXio and it´s dependencies
#endif

#endif
