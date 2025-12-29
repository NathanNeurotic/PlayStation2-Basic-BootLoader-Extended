
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

**Config search order (per build):**

1. `mc0:/SYS-CONF/PS2BBL.INI`
2. `mc1:/SYS-CONF/PS2BBL.INI`
3. `mass:/PS2BBL/CONFIG.INI`
4. `massX:/PS2BBL/CONFIG.INI` (when MX4SIO is built in)
5. `hdd0:__sysconf:pfs:/PS2BBL/CONFIG.INI` (when `HDD` *or* `HDD_RUNTIME` is built in)
6. `xfrom:/PS2BBL/CONFIG.INI` (when XFROM is built in)
7. `mmce0:/PS2BBL/PS2BBL.INI` then `mmce1:/PS2BBL/PS2BBL.INI` (when MMCE is built in)
8. `mc?:/SYS-CONF/PSXBBL.INI` (PSX builds)
9. `CONFIG.INI` in the current directory

If no config is found, built-in defaults are used.

**Runtime options of note (keys inside the INI):**
- `HDD_ENABLE=1` (only when built with `HDD_RUNTIME=1`): brings up the external HDD stack (DEV9, POWEROFF, ATAD, HDD, PFS) at runtime.
- `LOAD_IRX_E#=<path>`: loads an IRX from the given path after config parsing (e.g., extra device drivers).
- `LK_<BUTTON>_E#=<path>`: bind launch paths to controller buttons (AUTO/SELECT/L3/.../SQUARE).
- `SKIP_PS2LOGO`, `KEY_READ_WAIT_TIME`, `EJECT_TRAY`, `LOGO_DISPLAY`: standard boot behavior knobs.

**Build-time flags (Makefile) relevant to storage:**
- `HDD=1`: compile in embedded HDD modules (poweroff/atad/hdd/pfs) and auto-enable HDD support.
- `HDD_RUNTIME=1`: allow runtime HDD enablement from config (external IRX expected on memory card).
- `HAS_EMBED_IRX=1`: embed USB/bdm IRXs; otherwise they are loaded externally from memory card.

## Known bugs/issues

you tell me ;)

## Credits

- thanks to @SP193 for the OSD initialization libraries, wich serve as the foundation for this project
- thanks asmblur, for encouraging me to make this monster on latest sdk
- thanks to @uyjulian and @fjtrujy for always helping me
