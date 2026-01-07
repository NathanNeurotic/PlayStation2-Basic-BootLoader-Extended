#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

#include "tamtypes.h"

#ifndef MC_TYPE_MC
#define MC_TYPE_MC 0
#endif

#ifndef MC_TYPE_XMC
#define MC_TYPE_XMC 1
#endif

#ifndef sceMcTypePS2
#define sceMcTypePS2 2
#endif

#ifndef sceMcResSucceed
#define sceMcResSucceed 0
#endif

#ifndef sceMcResChangedCard
#define sceMcResChangedCard 1
#endif

#ifndef MC_FORMATTED
#define MC_FORMATTED 1
#endif

int mcInit(int type);
int mcGetInfo(int port, int slot, int *type, int *free, int *format);
int mcOpen(int port, int slot, const char *path, int mode);
int mcClose(int fd);
int mcSeek(int fd, int offset, int origin);
int mcRead(int fd, void *buffer, int size);
int mcWrite(int fd, const void *buffer, int size);
int mcGetDir(int port, int slot, const char *path, int flags, int max_entries, void *mc_table);
int mcMkDir(int port, int slot, const char *path);
int mcSetFileInfo(int port, int slot, const char *path, void *info, int flags);
int mcDelete(int port, int slot, const char *path);
int mcSync(int mode, int *cmd, int *result);
