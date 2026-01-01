#include "main.h"

#ifdef DEV9
int dev9_loaded = 0;
#endif

#if defined(HDD) || defined(HDD_RUNTIME)
char PART[128] = "\0";
int HDD_USABLE = 0;
#endif
