
<br />
<p align="center">
  <a href="https://israpps.github.io/PlayStation2-Basic-BootLoader/">
    <img width="1536" height="1024" alt="ChatGPT Image Dec 28, 2025, 07_29_41 PM" src="https://github.com/user-attachments/assets/e5c5fe17-6fe8-43bd-a581-105d3954f6f5"
 alt="Logo" width="100%" height="auto">
  </a>

  <p align="center">
    A flexible BootLoader for PlayStation 2™ and PSX-DESR
    <br />
  </p>
</p>  

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4ea4628e3d444807bf5df8430a327c5b)](https://www.codacy.com/gh/israpps/PlayStation2-Basic-BootLoader/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=israpps/PlayStation2-Basic-BootLoader&amp;utm_campaign=Badge_Grade)
[![CI](https://github.com/israpps/PlayStation2-Basic-BootLoader/actions/workflows/CI.yml/badge.svg?branch=main)](https://github.com/israpps/PlayStation2-Basic-BootLoader/actions/workflows/CI.yml)
![GitHub all releases](https://img.shields.io/github/downloads/israpps/PlayStation2-Basic-BootLoader/total?logo=GitHub&logoColor=white)



A simple PS2 (and PSX-DESR) bootloader that handles system init and ELF programs execution (amongst other things)

## Documentation

It is hosted on [github pages](https://israpps.github.io/PlayStation2-Basic-BootLoader/)

## Configuration paths and options

**Config search order (activated sources only — earliest wins):**

1. `CONFIG.INI` in the current directory (relative)
2. `mc1:/SYS-CONF/PS2BBL.INI`
3. `mc0:/SYS-CONF/PS2BBL.INI`
4. `mc?:/SYS-CONF/PSXBBL.INI` (PSX builds)
5. `mmce1:/PS2BBL/PS2BBL.INI` then `mmce0:/PS2BBL/PS2BBL.INI` (once the MMCE driver is running)
6. `xfrom:/PS2BBL/CONFIG.INI` (after XFROM is enabled at runtime)
7. `hdd0:__sysconf:pfs:/PS2BBL/CONFIG.INI` (after the HDD stack finishes init)
8. `massX:/PS2BBL/CONFIG.INI` (once MX4SIO is enabled)
9. `mass:/PS2BBL/CONFIG.INI`

If no config is found, built-in defaults are used:

- `AUTO`: `mc?:/BOOT/BOOT.ELF`, `mc?:/BOOT/BOOT2.ELF`, `mass:/RESCUE.ELF`
- `SELECT`: `mass:/PS2BBL/L2[1].ELF`, `mass:/PS2BBL/L2[2].ELF`, `mass:/PS2BBL/L2[3].ELF`
- `L3`: `mass:/PS2BBL/R2[1].ELF`, `mass:/PS2BBL/R2[2].ELF`, `mass:/PS2BBL/R2[3].ELF`
- `R3`: `mc?:/OPL/OPNPS2LD.ELF`, `mc?:/APPS/OPNPS2LD/ELF`, `mass:/PS2BBL/OPNPS2LD.ELF`
- `START`: `mass:/RESCUE.ELF`, `mc?:/BOOT/BOOT2.ELF`, `mc?:/APPS/ULE.ELF`

**Runtime options of note (keys inside the INI):**
- `HDD_ENABLE=1` (requires `HDD_RUNTIME=1` build): brings up the external HDD stack (DEV9 → POWEROFF → ATAD → HDD → PFS) and only then enables the `hdd0:__sysconf:pfs:/PS2BBL/CONFIG.INI` search slot.
- `MX4SIO_ENABLE=1` (requires `MX4SIO` or `MX4SIO_RUNTIME` build): loads `mx4sio_bd.irx` via the IRX locator (MC paths below), initializes the driver, and then enables `massX:/PS2BBL/CONFIG.INI`.
- `MMCE_ENABLE=1` (requires `MMCE` or `MMCE_RUNTIME` build): loads `mmceman.irx`, initializes the memory card extender, and then enables `mmce1:/PS2BBL/PS2BBL.INI` followed by `mmce0:/PS2BBL/PS2BBL.INI`.
- `XFROM_ENABLE=1` (requires `XFROM` or `XFROM_RUNTIME` build): loads `xfromman.irx`, initializes the device, and then enables `xfrom:/PS2BBL/CONFIG.INI`.
- `LOAD_IRX_E#=<path>`: load an extra IRX at config-parse time (e.g., `LOAD_IRX_E1=mc0:/SYS-CONF/PS2BBL/FSCK.IRX`). This **does not** add devices to the config search order; use the device `*_ENABLE` keys above for runtime device activation.
- `LK_<BUTTON>_E#=<path>`: bind launch paths to controller buttons (AUTO/SELECT/L3/.../SQUARE).
- `SKIP_PS2LOGO`, `KEY_READ_WAIT_TIME`, `EJECT_TRAY`, `LOGO_DISPLAY`: standard boot behavior knobs (logo skip, input timeout, tray control, logo verbosity).

`*_ENABLE` keys only bring their devices online **after** the initial config parse is done. They cannot be used to read the current boot’s `CONFIG.INI` from those newly enabled devices. If you need runtime overrides from those sources, chain-load a secondary config (e.g., via a launch entry that re-enters PS2BBL) or perform a second config pass after the device is up.

**Build-time flags (Makefile) relevant to storage and runtime devices:**
- `HDD=1`: embed HDD modules (POWEROFF/ATAD/HDD/PFS) and enable HDD by default.
- `HDD_RUNTIME=1`: allow runtime HDD enablement from external IRX on MC (pair with `HDD_ENABLE=1` in the INI to actually activate).
- `MX4SIO=1`, `MMCE=1`, `XFROM=1`: embed the respective drivers and enable their config search slots immediately after boot.
- `MX4SIO_RUNTIME=1`, `MMCE_RUNTIME=1`, `XFROM_RUNTIME=1`: allow runtime bring-up of the corresponding drivers from external IRX without embedding them; activation is controlled by the matching `*_ENABLE=1` INI keys.
- `HAS_EMBED_IRX=1`: embed USB/BDM IRXs; otherwise they are loaded externally from memory card.

### External IRX lookup

Runtime-loaded IRX modules (HDD/MX4SIO/MMCE/XFROM and other optional drivers) use a common locator that searches memory cards in this order: `mc0:/SYS-CONF/PS2BBL/<IRX>`, `mc1:/SYS-CONF/PS2BBL/<IRX>`, then `mc?:/SYS-CONF/<IRX>`. The USB/BDM stack still loads from `mc?:/SYS-CONF/<IRX>` when not embedded.

IRX filenames used by this project:

- Core input/memory card: `sio2man.irx`, `mcman.irx`, `mcserv.irx`, `padman.irx` (ROM or embedded depending on build).
- File I/O: `iomanX.irx`, `fileXio.irx`.
- USB/BDM stack (external when not embedded): `BDM.IRX`, `BDMFS_FATFS.IRX`, `USBD.IRX`, `USBMASS_BD.IRX`.
- HDD runtime stack: `PS2DEV9.IRX`, `POWEROFF.IRX`, `PS2ATAD.IRX`, `PS2HDD.IRX`, `PS2FS.IRX`.
- Optional runtime devices: `mx4sio_bd.irx`, `mmceman.irx`, `xfromman.irx`.
- Networking/debug (when enabled at build time): `netman.irx`, `smap.irx`, `ps2ip-nm.irx`, `udptty.irx`, `ppctty.irx`.

**Example CONFIG.INI (runtime devices + extra IRX):**
```
HDD_ENABLE=1
MX4SIO_ENABLE=1
MMCE_ENABLE=1
XFROM_ENABLE=1
LOAD_IRX_E1=mc0:/SYS-CONF/PS2BBL/FSCK.IRX    ; arbitrary extra module

; Button bindings
LK_AUTO_E1=mc0:/BOOT/BOOT.ELF
LK_AUTO_E2=mc0:/BOOT/BOOT2.ELF
LK_AUTO_E3=mass:/RESCUE.ELF
LK_START_E1=mass:/RESCUE.ELF

; Optional delay and UI tweaks
KEY_READ_WAIT_TIME=5000
LOGO_DISPLAY=2
```
For runtime driver lines above, place `mx4sio_bd.irx`, `mmceman.irx`, `xfromman.irx`, and the HDD stack IRX files inside `mc0:/SYS-CONF/PS2BBL/` (or the fallback MC paths listed under External IRX lookup).

## Known bugs/issues

you tell me ;)

## Credits

- thanks to @SP193 for the OSD initialization libraries, wich serve as the foundation for this project
- thanks asmblur, for encouraging me to make this monster on latest sdk
- thanks to @uyjulian and @fjtrujy for always helping me
- thanks to @israpps (El_isra) for creating and maintaining PS2BBL
- thanks to @pcm720 for contributions and reviews

## Build & usage notes

- Build requirements: ps2sdk toolchain installed and in `PATH`.
- Typical build: `make` (add `HDD=1` for embedded HDD stack, or `HDD_RUNTIME=1`/`MX4SIO_RUNTIME=1`/`MMCE_RUNTIME=1`/`XFROM_RUNTIME=1` to enable runtime drivers loaded from memory card).
- Release packaging: `./mk_kelf.sh` will produce KELF variants; external IRX for runtime HDD or USB must reside in `mc?:/SYS-CONF/`.
- Config file: place `CONFIG.INI` in the current directory (checked first) or any of the search paths listed above.
