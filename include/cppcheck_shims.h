#ifndef CPPCHECK_SHIMS_H
#define CPPCHECK_SHIMS_H

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef __SIZE_TYPE__
typedef unsigned int size_t;
#else
typedef __SIZE_TYPE__ size_t;
#endif

#ifndef __PTRDIFF_TYPE__
typedef int ptrdiff_t;
#else
typedef __PTRDIFF_TYPE__ ptrdiff_t;
#endif

#ifndef __UINT8_TYPE__
typedef unsigned char uint8_t;
#else
typedef __UINT8_TYPE__ uint8_t;
#endif

#ifndef __INT8_TYPE__
typedef signed char int8_t;
#else
typedef __INT8_TYPE__ int8_t;
#endif

#ifndef __UINT16_TYPE__
typedef unsigned short uint16_t;
#else
typedef __UINT16_TYPE__ uint16_t;
#endif

#ifndef __INT16_TYPE__
typedef signed short int16_t;
#else
typedef __INT16_TYPE__ int16_t;
#endif

#ifndef __UINT32_TYPE__
typedef unsigned int uint32_t;
#else
typedef __UINT32_TYPE__ uint32_t;
#endif

#ifndef __INT32_TYPE__
typedef signed int int32_t;
#else
typedef __INT32_TYPE__ int32_t;
#endif

#ifndef __UINT64_TYPE__
typedef unsigned long long uint64_t;
#else
typedef __UINT64_TYPE__ uint64_t;
#endif

#ifndef __INT64_TYPE__
typedef signed long long int64_t;
#else
typedef __INT64_TYPE__ int64_t;
#endif

#if defined(UINTPTR_MAX) || defined(__UINTPTR_TYPE__)
#ifndef __UINTPTR_TYPE__
typedef unsigned long uintptr_t;
#else
typedef __UINTPTR_TYPE__ uintptr_t;
#endif
#endif

#if defined(INTPTR_MAX) || defined(__INTPTR_TYPE__)
#ifndef __INTPTR_TYPE__
typedef long intptr_t;
#else
typedef __INTPTR_TYPE__ intptr_t;
#endif
#endif

#include "ps2sdk_shim.h"

#ifndef SCECdINoD
#define SCECdINoD 0
#endif

#ifndef SCECdEXIT
#define SCECdEXIT 1
#endif

#ifndef TV_SCREEN_43
#define TV_SCREEN_43 0
#endif

#ifndef VIDEO_OUTPUT_RGB
#define VIDEO_OUTPUT_RGB 0
#endif

#ifndef VIDEO_OUTPUT_COMPONENT
#define VIDEO_OUTPUT_COMPONENT 1
#endif

#ifndef LANGUAGE_JAPANESE
#define LANGUAGE_JAPANESE 0
#endif

#ifndef LANGUAGE_ENGLISH
#define LANGUAGE_ENGLISH 1
#endif

#ifndef LANGUAGE_FRENCH
#define LANGUAGE_FRENCH 2
#endif

#ifndef LANGUAGE_SPANISH
#define LANGUAGE_SPANISH 3
#endif

#ifndef LANGUAGE_GERMAN
#define LANGUAGE_GERMAN 4
#endif

#ifndef LANGUAGE_ITALIAN
#define LANGUAGE_ITALIAN 5
#endif

#ifndef LANGUAGE_DUTCH
#define LANGUAGE_DUTCH 6
#endif

#ifndef LANGUAGE_PORTUGUESE
#define LANGUAGE_PORTUGUESE 7
#endif

#ifndef LANGUAGE_RUSSIAN
#define LANGUAGE_RUSSIAN 8
#endif

#ifndef LANGUAGE_KOREAN
#define LANGUAGE_KOREAN 9
#endif

#ifndef LANGUAGE_TRAD_CHINESE
#define LANGUAGE_TRAD_CHINESE 10
#endif

#ifndef LANGUAGE_SIMPL_CHINESE
#define LANGUAGE_SIMPL_CHINESE 11
#endif

typedef struct ConfigParam
{
    int spdifMode;
    int screenType;
    int videoOutput;
    int japLanguage;
    int ps1drvConfig;
    int version;
    int language;
    int timezoneOffset;
} ConfigParam;

typedef struct Config2Param
{
    int format;
    int daylightSaving;
    int timeFormat;
    int dateFormat;
    int version;
    int language;
} Config2Param;

static inline void SetOsdConfigParam(const ConfigParam *config)
{
    (void)config;
}

static inline void GetOsdConfigParam(ConfigParam *config)
{
    (void)config;
}

static inline void SetOsdConfigParam2(const Config2Param *config, int size, int offset)
{
    (void)config;
    (void)size;
    (void)offset;
}

static inline void GetOsdConfigParam2(Config2Param *config, int size, int offset)
{
    (void)config;
    (void)size;
    (void)offset;
}

static inline void SetGsVParam(int mode)
{
    (void)mode;
}

#endif
