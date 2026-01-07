#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

typedef unsigned short u16;
typedef unsigned int u32;

typedef struct padButtonStatus {
    u16 btns;
    u16 _pad;
    u32 btns2;
} padButtonStatus;

int padInit(int mode);
int padPortOpen(int port, int slot, void *pad_area);
int padPortClose(int port, int slot);
int padEnd(void);
int padRead(int port, int slot, struct padButtonStatus *buttons);
