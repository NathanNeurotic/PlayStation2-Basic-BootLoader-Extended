#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

int fileXioInit(void);
int fileXioDopen(const char *path);
int fileXioDclose(int fd);
int fileXioIoctl(int fd, int cmd, void *arg);
int fileXioDevctl(const char *name, int cmd, void *arg, unsigned int arglen, void *buf, unsigned int buflen);
int fileXioMount(const char *mnt, const char *dev, int flag);
int fileXioUmount(const char *mnt);
