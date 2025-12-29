#ifndef COMMONDEF
#define COMMONDEF
enum
{
    SOURCE_MC0 = 0,
    SOURCE_MC1,
    SOURCE_MASS,
#ifdef MX4SIO
    SOURCE_MX4SIO,
#endif
#ifdef HDD
    SOURCE_HDD,
#endif
#ifdef XFROM
    SOURCE_XFROM,
#endif
#ifdef MMCE
    SOURCE_MMCE1,
    SOURCE_MMCE0,
#endif
#ifdef PSX
    SOURCE_XCONFIG,
#endif
    SOURCE_CWD,
    SOURCE_INVALID,
    SOURCE_COUNT,
} CONFIG_SOURCES_ID;

extern const char *CONFIG_PATHS[SOURCE_COUNT];

static const char *SOURCES[SOURCE_COUNT] = {
    "mc0",
    "mc1",
    "usb",
#ifdef MX4SIO
    "mx4sio",
#endif
#ifdef HDD
    "hdd",
#endif
#ifdef XFROM
    "xfrom",
#endif
#ifdef MMCE
    "mmce0",
    "mmce1",
#endif
#ifdef PSX
    "XCONF",
#endif
    "CWD",
    "NOT FOUND",
};

#define MAX_LEN     64
#define CNF_LEN_MAX 20480 // 20kb should be enough for massive CNF's

/** default ammount of time this program will wait for a key press*/
#define DEFDELAY 5000

/** dualshock keys enumerator */
enum
{
    AUTO,
    SELECT,
    L3,
    R3,
    START,
    UP,
    RIGHT,
    DOWN,
    LEFT,
    L2,
    R2,
    L1,
    R1,
    TRIANGLE,
    CIRCLE,
    CROSS,
    SQUARE
} KEYS;

/** string alias of dualshock keys for config file */
const char *KEYS_ID[17] = {
    "AUTO",
    "SELECT",   // 0x0001
    "L3",       // 0x0002
    "R3",       // 0x0004
    "START",    // 0x0008
    "UP",       // 0x0010
    "RIGHT",    // 0x0020
    "DOWN",     // 0x0040
    "LEFT",     // 0x0080
    "L2",       // 0x0100
    "R2",       // 0x0200
    "L1",       // 0x0400
    "R1",       // 0x0800
    "TRIANGLE", // 0x1000
    "CIRCLE",   // 0x2000
    "CROSS",    // 0x4000
    "SQUARE"    // 0x8000
};

/** default paths used if config file can't be loaded */
extern const char *DEFPATH[];

#ifndef COMMIT_HASH
#define COMMIT_HASH "UNKNOWn"
#endif

#endif // COMMONDEF
