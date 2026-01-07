#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

#include "tamtypes.h"

#ifndef SCECdINoD
#define SCECdINoD 0
#endif

#ifndef SCECdEXIT
#define SCECdEXIT 1
#endif

typedef struct sceCdCLOCK {
    u8 stat;
    u8 second;
    u8 minute;
    u8 hour;
    u8 pad[4];
} sceCdCLOCK;

int sceCdInit(int mode);
int sceCdReadClock(sceCdCLOCK *clock);
int sceCdOpenConfig(int arg0, int arg1, int arg2, u32 *stat);
int sceCdReadConfig(u8 *config, u32 *stat);
int sceCdWriteConfig(const u8 *config, u32 *stat);
int sceCdCloseConfig(u32 *stat);
int sceCdReadKey(int key, int index, int keynum, u8 *buffer);
int sceCdGetError(void);
int sceCdApplySCmd(int cmd, const void *in, int in_size, void *out);
int sceCdMV(u8 *data, u32 *status);
int sceCdAutoAdjustCtrl(int mode, u32 *stat);
int sceCdGetDiskType(void);
int sceCdTrayReq(int mode, u32 *stat);
int sceCdForbidDVDP(u32 *stat);
