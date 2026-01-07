#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

#include "tamtypes.h"

#ifndef UNCACHED_SEG
#define UNCACHED_SEG(x) (x)
#endif

int AddIntcHandler(int interrupt, int (*handler)(int), int next);
int RemoveIntcHandler(int interrupt, int id);
int EnableIntc(int interrupt);
int DisableIntc(int interrupt);
int SetAlarm(u32 ticks, void (*callback)(s32 alarm_id, u16 time, void *common), void *common);
int GetThreadId(void);
void SignalSema(int sema_id);
