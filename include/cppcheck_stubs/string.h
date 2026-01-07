#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

typedef unsigned long size_t;

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);
char *strdup(const char *s);
