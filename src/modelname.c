#include "platform_includes.h"
#include "libcdvd_add.h"

#include "modelname.h"
#include "util_safe.h"

#define MODEL_NAME_MAX_LEN 17
static char ModelName[MODEL_NAME_MAX_LEN];

extern char ConsoleROMVER[];

static int ReadModelName(char *name)
{
    int result;
    u32 stat;

    /*  This function is a hybrid between the late ROM browser program and the HDD Browser.
        In v2.20, there was only a simple null-terminate before calling sceCdRM(), as below.
        However, this does not support the early PlayStation 2 models.
        In the HDD Browser, the model name buffer was a global instead of an argument.    */
    memset(name, 0, MODEL_NAME_MAX_LEN);

    /*      Invoking the "Read model" command on the first SCPH-10000 seems to return an error code,
            but it seems to function on my SCPH-15000...although no valid name is returned!
            In the HDD OSD, "SCPH-10000" is returned when the ROM v1.00 is detected (It checks rom0:ROMVER).
            It attempts to read the model name from the ROM OSDSYS program, when ROM v1.01 (late SCPH-10000 and all SCPH-15000 models) is detected.

            The model name was originally hardcoded into the OSDSYS. The MECHACON EEPROM did not have the model name recorded in it.

            Oddly, the console models that come with v1.01, like the SCPH-15000 and DTL-H10000,
            will always be identified as "SCPH-10000" by their ROM OSDSYS programs.
    */
    if (ConsoleROMVER[0] == '0' && ConsoleROMVER[1] == '1' && ConsoleROMVER[2] == '0') {
        if (ConsoleROMVER[3] == '0') // For ROM v1.00 (Early SCPH-10000 units).
            util_strlcpy(name, "SCPH-10000", MODEL_NAME_MAX_LEN);
        else { // For ROM v1.01 (Late SCPH-10000, and all SCPH-15000 units).
            int fd;
            if ((fd = open("rom0:OSDSYS", O_RDONLY)) >= 0) { // The model name is located at this address.
                lseek(fd, 0x8C808, SEEK_SET);
                // Use bounded read helper for Codacy CWE-120/CWE-20 compliance.
                ssize_t len = safe_read_once_nt(fd, name, MODEL_NAME_MAX_LEN);
                if (len <= 0) { // Treat EOF/error as empty string to preserve prior behavior.
                    name[0] = '\0';
                }
                close(fd);
            } else
                util_strlcpy(name, "Unknown", MODEL_NAME_MAX_LEN);
        }

        return 0; // Original returned -1
    } else {
        if ((result = sceCdRM(name, &stat)) == 1) { // Command issued successfully.
            if (stat & 0x80)
                return -2;
            if ((stat & 0x40) || name[0] == '\0')
                util_strlcpy(name, "unknown", MODEL_NAME_MAX_LEN);

            return 0; // Original returned -1
        } else
            return -2;
    }
}

int ModelNameInit(void)
{
    return ReadModelName(ModelName);
}

const char *ModelNameGet(void)
{
    return ModelName;
}
