
<br />
<p align="center">
  <a href="https://israpps.github.io/PlayStation2-Basic-BootLoader/">
    <img src="https://israpps.github.io/PlayStation2-Basic-BootLoader/logo.png" alt="Logo" width="100%" height="auto">
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
2. `mc?:/SYS-CONF/PSXBBL.INI` (PSX builds)
3. `mmce1:/PS2BBL/PS2BBL.INI` then `mmce0:/PS2BBL/PS2BBL.INI` (once the MMCE driver is running)
4. `xfrom:/PS2BBL/CONFIG.INI` (after XFROM is enabled at runtime)
5. `hdd0:__sysconf:pfs:/PS2BBL/CONFIG.INI` (after the HDD stack finishes init)
6. `massX:/PS2BBL/CONFIG.INI` (once MX4SIO is enabled)
7. `mass:/PS2BBL/CONFIG.INI`
8. `mc1:/SYS-CONF/PS2BBL.INI`
9. `mc0:/SYS-CONF/PS2BBL.INI`

If no config is found, built-in defaults are used:

- `AUTO`: `mc?:/BOOT/ULE.ELF`, `mc?:/APPS/ULE/ELF`, `mass:/BOOT/BOOT.ELF`
- `SELECT`: `mass:/PS2BBL/L2[1].ELF`, `mass:/PS2BBL/L2[2].ELF`, `mass:/PS2BBL/L2[3].ELF`
- `L3`: `mass:/PS2BBL/R2[1].ELF`, `mass:/PS2BBL/R2[2].ELF`, `mass:/PS2BBL/R2[3].ELF`
- `R3`: `mc?:/OPL/OPNPS2LD.ELF`, `mc?:/APPS/OPNPS2LD/ELF`, `mass:/PS2BBL/OPNPS2LD.ELF`
- `START`: `mass:/RESCUE.ELF`, `mc?:/BOOT/BOOT2.ELF`, `mc?:/APPS/ULE.ELF`

**Runtime options of note (keys inside the INI):**
- `HDD_ENABLE=1` (only when built with `HDD_RUNTIME=1`): brings up the external HDD stack (DEV9, POWEROFF, ATAD, HDD, PFS) at runtime and enables the `hdd0:` config path.
- `MX4SIO_ENABLE=1` (when built with `MX4SIO` or `MX4SIO_RUNTIME`): loads `mx4sio_bd.irx` from memory card search paths and enables `massX:/` in the config search order.
- `MMCE_ENABLE=1` (when built with `MMCE` or `MMCE_RUNTIME`): loads `mmceman.irx` from memory card search paths and enables `mmce1:/` then `mmce0:/` in the config search order.
- `XFROM_ENABLE=1` (when built with `XFROM` or `XFROM_RUNTIME`): loads `xfromman.irx` from memory card search paths and enables `xfrom:/` in the config search order.
- `LOAD_IRX_E#=<path>`: loads an IRX from the given path after config parsing (e.g., extra device drivers). This does **not** change which devices are searched for configs; use the `*_ENABLE` options above to add runtime devices to the lookup list.
- `LK_<BUTTON>_E#=<path>`: bind launch paths to controller buttons (AUTO/SELECT/L3/.../SQUARE).
- `SKIP_PS2LOGO`, `KEY_READ_WAIT_TIME`, `EJECT_TRAY`, `LOGO_DISPLAY`: standard boot behavior knobs.

**Build-time flags (Makefile) relevant to storage:**
- `HDD=1`: compile in embedded HDD modules (poweroff/atad/hdd/pfs) and auto-enable HDD support.
- `HDD_RUNTIME=1`: allow runtime HDD enablement from config (external IRX expected on memory card).
- `HAS_EMBED_IRX=1`: embed USB/bdm IRXs; otherwise they are loaded externally from memory card.
- `MX4SIO_RUNTIME=1`, `MMCE_RUNTIME=1`, `XFROM_RUNTIME=1`: allow runtime bring-up of the corresponding device drivers from external IRX files without embedding them.

### External IRX lookup

Runtime-loaded IRX modules (HDD/MX4SIO/MMCE/XFROM and other optional drivers) use a common locator that searches memory cards in this order: `mc0:/SYS-CONF/PS2BBL/<IRX>`, `mc1:/SYS-CONF/PS2BBL/<IRX>`, then `mc?:/SYS-CONF/<IRX>`. The USB/BDM stack still loads from `mc?:/SYS-CONF/<IRX>` when not embedded.

IRX filenames used by this project:

- Core input/memory card: `sio2man.irx`, `mcman.irx`, `mcserv.irx`, `padman.irx` (ROM or embedded depending on build).
- File I/O: `iomanX.irx`, `fileXio.irx`.
- USB/BDM stack (external when not embedded): `BDM.IRX`, `BDMFS_FATFS.IRX`, `USBD.IRX`, `USBMASS_BD.IRX`.
- HDD runtime stack: `PS2DEV9.IRX`, `POWEROFF.IRX`, `PS2ATAD.IRX`, `PS2HDD.IRX`, `PS2FS.IRX`.
- Optional runtime devices: `mx4sio_bd.irx`, `mmceman.irx`, `xfromman.irx`.
- Networking/debug (when enabled at build time): `netman.irx`, `smap.irx`, `ps2ip-nm.irx`, `udptty.irx`, `ppctty.irx`.

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
