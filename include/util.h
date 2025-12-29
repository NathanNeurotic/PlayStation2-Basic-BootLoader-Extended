#include <stddef.h>

/**
 * @brief checks for file existence
 * @param filepath path to the file to check
 * @returns 1 if exists, 0 if not
*/
int exist(char *filepath);

/**
 * @brief waits a specified ammount of time, usually used to provide the famous 'USB delay', wLaunchELF uses 3, KELFBinder used 5?
*/
void delay(int count);

/**
 * @brief load an IRX file into memory from a file
 * @param path path to the IRX
 * @param arg_len argc for IRX
 * @param args '\0' delimited list of args for IRX
 * @param mod_res see SifExecModuleBuffer documentation on SDK for this
 * @returns nonzero on failure, check SifExecModuleBuffer documentation on SDK
*/
int loadIRXFile(char *path, u32 arg_len, const char *args, int *mod_res);

/// @brief config loop iterator borrowed from wLaunchELF
int get_CNF_string(char **CNF_p_p,
                   char **name_p_p,
                   char **value_p_p);

/**
 * @brief  method returns 0 if it can extract needed info from path, otherwise a negative error code.
 * In case of success, it also updates mountString, mountPoint and newCWD parameters
 * It splits path by ":", and requires a minimum of 3 elements
 * @example if path = hdd0:__common:pfs:/retroarch/ then: mountString = "pfs:", mountPoint = "hdd0:__common", newCWD = pfs:/retroarch/
 * @param path input parameter with full hdd path (`hdd0:__common:pfs:/retroarch/`)
 * @param mountString pointer to char* wich will contain pfs mountpoint (`pfs:`)
 * @param mountStringSize total size of the mountString buffer
 * @param mountPoint returns the path of mounted partition (`hdd0:__common`)
 * @param mountPointSize total size of the mountPoint buffer
 * @param newCWD returns the path to the file as pfs mount point string
 * @param newCWDSize total size of the newCWD buffer
 * @return 0 on success, negative error code on failure or truncation
*/
int getMountInfo(char *path, char *mountString, size_t mountStringSize, char *mountPoint, size_t mountPointSize, char *newCWD, size_t newCWDSize);
