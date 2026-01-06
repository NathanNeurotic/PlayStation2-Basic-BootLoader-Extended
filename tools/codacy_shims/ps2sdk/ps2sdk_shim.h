#ifndef PS2SDK_SHIM_H
#define PS2SDK_SHIM_H

#if defined(__has_include) && __has_include(<stdbool.h>)
#include <stdbool.h>
#else
#include "../stdlib/stdbool.h"
#endif

#if defined(__has_include) && __has_include(<stddef.h>)
#include <stddef.h>
#else
#include "../stdlib/stddef.h"
#endif

#if defined(__has_include) && __has_include(<stdint.h>)
#include <stdint.h>
#else
#include "../stdlib/stdint.h"
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#ifndef ALIGNED
#define ALIGNED(x) __attribute__((aligned(x)))
#endif

#define SCECdNODISC 0
#define SCECdDETCT 1
#define SCECdDETCTCD 2
#define SCECdDETCTDVDS 3
#define SCECdDETCTDVDD 4
#define SCECdPS2CD 10
#define SCECdPS2CDDA 11
#define SCECdPS2DVD 12
#define SCECdPSCD 13
#define SCECdPSCDDA 14
#define SCECdDVDV 15
#define SCECdCDDA 16
#define SCECdEXIT 17
#define SCECdINoD 18
#define SCECdErREAD -1

#define sceDVRTrayModePS2 2

#define MC_TYPE_MC 0
#define MC_TYPE_XMC 1
#define MC_FORMATTED 2
#define sceMcTypePS2 2

#define USBMASS_IOCTL_GET_DRIVERNAME 0x0000
#define FIO_MT_RDONLY 1

typedef struct padButtonStatus
{
    u16 btns;
} padButtonStatus;

typedef struct SifRpcClientData
{
    int server;
    void *buff;
} SifRpcClientData_t;

typedef struct sceCdCLOCK
{
    u8 second;
    u8 minute;
    u8 hour;
    u8 week;
    u8 day;
    u8 month;
    u8 year;
} sceCdCLOCK;

static inline int AddIntcHandler(int irq, int (*handler)(int), int next)
{
    (void)irq;
    (void)handler;
    (void)next;
    return 0;
}

static inline int RemoveIntcHandler(int irq, int handler_id)
{
    (void)irq;
    (void)handler_id;
    return 0;
}

static inline void EnableIntc(int irq)
{
    (void)irq;
}

static inline void DisableIntc(int irq)
{
    (void)irq;
}

static inline int SetAlarm(int usec, void (*handler)(s32, u16, void *), void *common)
{
    (void)usec;
    (void)handler;
    (void)common;
    return 0;
}

static inline void SleepThread(void)
{
}

static inline int GetThreadId(void)
{
    return 0;
}

static inline void fioInit(void)
{
}

static inline int sbv_patch_enable_lmb(void)
{
    return 0;
}

static inline int sbv_patch_disable_prefix_check(void)
{
    return 0;
}

static inline void SifInitCmd(void)
{
}

static inline void SifExitCmd(void)
{
}

static inline int SifCallRpc(SifRpcClientData_t *client, int rpc, int mode, void *send, int ssize, void *recv, int rsize, void *cb, void *carg)
{
    (void)client;
    (void)rpc;
    (void)mode;
    (void)send;
    (void)ssize;
    (void)recv;
    (void)rsize;
    (void)cb;
    (void)carg;
    return 0;
}

static inline int SifExecModuleBuffer(const void *mod, int size, int argc, const char *argv, int *result)
{
    (void)mod;
    (void)size;
    (void)argc;
    (void)argv;
    (void)result;
    return 0;
}

static inline int SifLoadStartModule(const char *path, int argc, const char *argv, int *result)
{
    (void)path;
    (void)argc;
    (void)argv;
    (void)result;
    return 0;
}

static inline int SifLoadModule(const char *path, int argc, const char *argv)
{
    (void)path;
    (void)argc;
    (void)argv;
    return 0;
}

static inline int SifInitRpc(int mode)
{
    (void)mode;
    return 0;
}

static inline int SifIopReset(const char *arg, int mode)
{
    (void)arg;
    (void)mode;
    return 1;
}

static inline int SifIopSync(void)
{
    return 1;
}

static inline int SifIopRebootBuffer(const void *arg, int arglen)
{
    (void)arg;
    (void)arglen;
    return 1;
}

static inline int SifInitIopHeap(void)
{
    return 0;
}

static inline int SifLoadFileInit(void)
{
    return 0;
}

static inline int sceCdReadKey(int type, int unk, int command, u8 *dst)
{
    (void)type;
    (void)unk;
    (void)command;
    (void)dst;
    return 1;
}

static inline int sceCdGetError(void)
{
    return 0;
}

static inline int sceCdMV(u8 *out, u32 *stat)
{
    (void)out;
    (void)stat;
    return 0;
}

static inline int sceCdRM(char *out, u32 *stat)
{
    (void)out;
    (void)stat;
    return 0;
}

static inline int sceCdApplySCmd(int cmd, const void *in, unsigned int in_size, void *out)
{
    (void)cmd;
    (void)in;
    (void)in_size;
    (void)out;
    return 0;
}

static inline int sceCdReadRegionParams(u8 *data, u32 *stat)
{
    (void)data;
    (void)stat;
    return 0;
}

static inline int sceCdReadPS1BootParam(char *param, u32 *stat)
{
    (void)param;
    (void)stat;
    return 0;
}

static inline int sceCdRcBypassCtl(int bypass, u32 *stat)
{
    (void)bypass;
    (void)stat;
    return 0;
}

static inline int sceCdBootCertify(const u8 *data)
{
    (void)data;
    return 0;
}

static inline int sceCdInit(int mode)
{
    (void)mode;
    return 0;
}

static inline int sceCdChgSys(int mode)
{
    (void)mode;
    return mode;
}

static inline int sceCdNoticeGameStart(int mode, u32 *stat)
{
    (void)mode;
    (void)stat;
    return 0;
}

static inline int sceCdAutoAdjustCtrl(int enable)
{
    (void)enable;
    return 0;
}

static inline int sceCdGetDiskType(void)
{
    return SCECdNODISC;
}

static inline int sceCdTrayReq(int mode, int *traycnt)
{
    (void)mode;
    (void)traycnt;
    return 0;
}

static inline int sceCdReadClock(sceCdCLOCK *clock)
{
    if (clock != NULL) {
        clock->year = 0;
        clock->month = 1;
        clock->day = 1;
    }
    return 0;
}

static inline int sceCdOpenConfig(int block, int layer, int mode, void *stat)
{
    (void)block;
    (void)layer;
    (void)mode;
    (void)stat;
    return 1;
}

static inline int sceCdReadConfig(void *buf, void *stat)
{
    (void)buf;
    (void)stat;
    return 1;
}

static inline int sceCdWriteConfig(const void *buf, void *stat)
{
    (void)buf;
    (void)stat;
    return 1;
}

static inline int sceCdCloseConfig(void *stat)
{
    (void)stat;
    return 1;
}

static inline int sceCdForbidDVDP(u32 *stat)
{
    (void)stat;
    return 0;
}

static inline int padInit(int mode)
{
    (void)mode;
    return 0;
}

static inline int padEnd(void)
{
    return 0;
}

static inline int padPortOpen(int port, int slot, void *buf)
{
    (void)port;
    (void)slot;
    (void)buf;
    return 1;
}

static inline int padPortClose(int port, int slot)
{
    (void)port;
    (void)slot;
    return 0;
}

static inline int padRead(int port, int slot, padButtonStatus *status)
{
    (void)port;
    (void)slot;
    if (status != NULL) {
        status->btns = 0;
    }
    return 1;
}

static inline int mcInit(int type)
{
    (void)type;
    return 0;
}

static inline int mcGetInfo(int port, int slot, int *type, int *free)
{
    (void)port;
    (void)slot;
    if (type != NULL)
        *type = sceMcTypePS2;
    if (free != NULL)
        *free = 0;
    return 0;
}

static inline int mcMkDir(int port, int slot, const char *name)
{
    (void)port;
    (void)slot;
    (void)name;
    return 0;
}

static inline int mcSync(int mode, int *cmd, int *result)
{
    (void)mode;
    (void)cmd;
    if (result != NULL)
        *result = 0;
    return 0;
}

static inline int mcOpen(int port, int slot, const char *name, int mode)
{
    (void)port;
    (void)slot;
    (void)name;
    (void)mode;
    return 0;
}

static inline int mcClose(int fd)
{
    (void)fd;
    return 0;
}

static inline int mcRead(int fd, void *buffer, int size)
{
    (void)fd;
    (void)buffer;
    (void)size;
    return 0;
}

static inline int mcWrite(int fd, const void *buffer, int size)
{
    (void)fd;
    (void)buffer;
    (void)size;
    return 0;
}

static inline int mcSeek(int fd, int offset, int mode)
{
    (void)fd;
    (void)offset;
    (void)mode;
    return 0;
}

static inline int mcSetFileInfo(const char *name, void *info)
{
    (void)name;
    (void)info;
    return 0;
}

static inline int mcDelete(const char *name)

{
    (void)name;
    return 0;
}

static inline int fileXioInit(void)
{
    return 0;
}

static inline int fileXioDopen(const char *name)
{
    (void)name;
    return -1;
}

static inline int fileXioDclose(int fd)
{
    (void)fd;
    return 0;
}

static inline int fileXioIoctl(int fd, int cmd, void *arg)
{
    (void)fd;
    (void)cmd;
    (void)arg;
    return 0;
}

static inline int fileXioDevctl(const char *name, int cmd, void *arg, unsigned int arglen, void *buf, unsigned int buflen)
{
    (void)name;
    (void)cmd;
    (void)arg;
    (void)arglen;
    (void)buf;
    (void)buflen;
    return 0;
}

static inline int fileXioMount(const char *mount, const char *blockdev, int flags)
{
    (void)mount;
    (void)blockdev;
    (void)flags;
    return 0;
}

static inline int fileXioUmount(const char *mount)
{
    (void)mount;
    return 0;
}

static inline int poweroffInit(void)
{
    return 0;
}

static inline int poweroffSetCallback(void (*cb)(void *), void *arg)
{
    (void)cb;
    (void)arg;
    return 0;
}

static inline void poweroffShutdown(void)
{
}

static inline int sio_init(int port, int baud)
{
    (void)port;
    (void)baud;
    return 0;
}

static inline int sio_putsn(const char *str)
{
    (void)str;
    return 0;
}

static inline int scr_printf(const char *fmt, ...)
{
    (void)fmt;
    return 0;
}

static inline void scr_setfontcolor(int color)
{
    (void)color;
}

static inline void LoadELFFromFile(const char *path, int argc, const char *argv[])
{
    (void)path;
    (void)argc;
    (void)argv;
}

static inline void LoadELFFromFileWithPartition(const char *path, const char *party, int argc, const char *argv[])
{
    (void)path;
    (void)party;
    (void)argc;
    (void)argv;
}

#endif
