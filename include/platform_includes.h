#ifndef PLATFORM_INCLUDES_H
#define PLATFORM_INCLUDES_H

#ifndef __CPPCHECK__
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sifcmd.h>
#include <libcdvd.h>
#include <libmc.h>
#include <libpad.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <loadfile.h>
#include <sbv_patches.h>
#include <debug.h>
#include <osd_config.h>
#include <elf-loader.h>
#include <sio.h>
#include <ps2sdkapi.h>
#include <usbhdfsd-common.h>

#ifdef EE_SIO_DEBUG
#include <SIOCookie.h>
#endif

#ifdef PSX
#include <iopcontrol_special.h>
#endif

#if defined(HDD) || defined(HDD_RUNTIME)
#include <hdd-ioctl.h>
#include <io_common.h>
#include <libpwroff.h>
#endif

#ifdef FILEXIO
#include <fileXio_rpc.h>
#endif
#endif /* __CPPCHECK__ */

#endif /* PLATFORM_INCLUDES_H */
