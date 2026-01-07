#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

#ifndef __SIZE_TYPE__
typedef unsigned long size_t;
#else
typedef __SIZE_TYPE__ size_t;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif
