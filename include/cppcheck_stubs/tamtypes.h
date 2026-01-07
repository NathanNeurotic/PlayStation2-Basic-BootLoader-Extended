#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

#ifndef ALIGNED
#define ALIGNED(x) __attribute__((aligned(x)))
#endif
