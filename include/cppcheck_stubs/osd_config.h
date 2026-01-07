#pragma once
// Stub for Codacy/Cppcheck static analysis; not used in PS2SDK builds.

typedef struct ConfigParam {
    int spdifMode;
    int screenType;
    int videoOutput;
    int japLanguage;
    int ps1drvConfig;
    int version;
    int language;
    int timezoneOffset;
} ConfigParam;

typedef struct Config2Param {
    int format;
    int daylightSaving;
    int timeFormat;
    int dateFormat;
    int version;
    int language;
} Config2Param;

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

void SetOsdConfigParam(const ConfigParam *config);
void GetOsdConfigParam(ConfigParam *config);
void SetOsdConfigParam2(const Config2Param *config, int size, int offset);
void GetOsdConfigParam2(Config2Param *config, int size, int offset);
