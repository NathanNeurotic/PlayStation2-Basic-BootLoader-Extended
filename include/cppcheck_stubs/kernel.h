#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

#ifndef UNCACHED_SEG
#define UNCACHED_SEG(x) (x)
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

int AddIntcHandler(int interrupt, int (*handler)(int), int next);
int RemoveIntcHandler(int interrupt, int id);
int EnableIntc(int interrupt);
int DisableIntc(int interrupt);
int SetAlarm(u32 ticks, void (*callback)(s32 alarm_id, u16 time, void *common), void *common);
int GetThreadId(void);
void SignalSema(int sema_id);
