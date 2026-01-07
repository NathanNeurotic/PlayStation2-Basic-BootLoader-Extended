#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

int SifLoadFileInit(void);
int SifLoadModule(const char *path, int argc, const char *argv);
int SifLoadStartModule(const char *path, int argc, const char *argv, int *ret);
int SifExecModuleBuffer(const void *modptr, int size, int argc, const char *argv, int *ret);
