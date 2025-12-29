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
#if defined(HDD) || defined(HDD_RUNTIME)
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

extern const char *const CONFIG_PATHS[SOURCE_COUNT];
extern const char *const SOURCES[SOURCE_COUNT];

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
extern const char *const KEYS_ID[17];

/** default paths used if config file can't be loaded */
extern const char *const DEFPATH[];

#ifndef COMMIT_HASH
#define COMMIT_HASH "UNKNOWn"
#endif

#endif // COMMONDEF
