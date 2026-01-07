#ifndef PLATFORM_INCLUDES_H
#define PLATFORM_INCLUDES_H

#ifndef __CPPCHECK__
/* Note: keep PS2SDK headers behind PLATFORM_HAS_INCLUDE so static analyzers without
 * the SDK available can parse code safely without impacting real PS2SDK builds.
 */
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

#if defined(__has_include)
#define PLATFORM_HAS_INCLUDE(header) __has_include(header)
#else
#define PLATFORM_HAS_INCLUDE(header) 1
#endif

#if PLATFORM_HAS_INCLUDE(<tamtypes.h>)
#include <tamtypes.h>
#endif
#if PLATFORM_HAS_INCLUDE(<kernel.h>)
#include <kernel.h>
#endif
#if PLATFORM_HAS_INCLUDE(<sifrpc.h>)
#include <sifrpc.h>
#endif
#if PLATFORM_HAS_INCLUDE(<sifcmd.h>)
#include <sifcmd.h>
#endif
#if PLATFORM_HAS_INCLUDE(<libcdvd.h>)
#include <libcdvd.h>
#endif
#if PLATFORM_HAS_INCLUDE(<libmc.h>)
#include <libmc.h>
#endif
#if PLATFORM_HAS_INCLUDE(<libpad.h>)
#include <libpad.h>
#endif
#if PLATFORM_HAS_INCLUDE(<iopcontrol.h>)
#include <iopcontrol.h>
#endif
#if PLATFORM_HAS_INCLUDE(<iopheap.h>)
#include <iopheap.h>
#endif
#if PLATFORM_HAS_INCLUDE(<loadfile.h>)
#include <loadfile.h>
#endif
#if PLATFORM_HAS_INCLUDE(<sbv_patches.h>)
#include <sbv_patches.h>
#endif
#if PLATFORM_HAS_INCLUDE(<debug.h>)
#include <debug.h>
#endif
#if PLATFORM_HAS_INCLUDE(<osd_config.h>)
#include <osd_config.h>
#endif
#if PLATFORM_HAS_INCLUDE(<elf-loader.h>)
#include <elf-loader.h>
#endif
#if PLATFORM_HAS_INCLUDE(<sio.h>)
#include <sio.h>
#endif
#if PLATFORM_HAS_INCLUDE(<ps2sdkapi.h>)
#include <ps2sdkapi.h>
#endif
#if PLATFORM_HAS_INCLUDE(<usbhdfsd-common.h>)
#include <usbhdfsd-common.h>
#endif

#ifdef EE_SIO_DEBUG
#if PLATFORM_HAS_INCLUDE(<SIOCookie.h>)
#include <SIOCookie.h>
#endif
#endif

#ifdef PSX
#if PLATFORM_HAS_INCLUDE(<iopcontrol_special.h>)
#include <iopcontrol_special.h>
#endif
#endif

#if defined(HDD) || defined(HDD_RUNTIME)
#if PLATFORM_HAS_INCLUDE(<hdd-ioctl.h>)
#include <hdd-ioctl.h>
#endif
#if PLATFORM_HAS_INCLUDE(<io_common.h>)
#include <io_common.h>
#endif
#if PLATFORM_HAS_INCLUDE(<libpwroff.h>)
#include <libpwroff.h>
#endif
#endif

#ifdef FILEXIO
#if PLATFORM_HAS_INCLUDE(<fileXio_rpc.h>)
#include <fileXio_rpc.h>
#endif
#endif

#undef PLATFORM_HAS_INCLUDE
#endif /* __CPPCHECK__ */

#endif /* PLATFORM_INCLUDES_H */
