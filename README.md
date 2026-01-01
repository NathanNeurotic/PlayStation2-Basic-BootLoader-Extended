<br />
<p align="center">
  <a href="https://israpps.github.io/PlayStation2-Basic-BootLoader/">
    <img width="1536" height="1024" alt="PS2BBL Logo" src="https://github.com/user-attachments/assets/06f5c8a7-37c0-4e69-bd20-3fc890f045e6" />
  </a>
</p>

<p align="center">
  A flexible BootLoader for PlayStation 2™ and PSX-DESR
</p>

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/8a5773aa30cf4aca97550ec8053908d6)](
https://app.codacy.com/gh/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/dashboard
)
[![CI](https://github.com/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/actions/workflows/CI.yml/badge.svg?branch=main)](
https://github.com/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/actions/workflows/CI.yml
)
![GitHub all releases](https://img.shields.io/github/downloads/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/total?logo=GitHub&logoColor=white)


---

## Overview

PS2BBL is a lightweight yet powerful bootloader for **PlayStation 2™** and **PSX-DESR** systems.  
It performs early system initialization and launches ELF executables from multiple storage backends, supporting both **embedded** and **runtime-loaded** drivers.

The boot process is deterministic, with a clearly defined configuration search order and strict separation between **boot-time** and **runtime-enabled** devices.

---

## Documentation

Full original documentation is available on GitHub Pages:  
👉 https://israpps.github.io/PlayStation2-Basic-BootLoader/  

This Repo is Forked from:  

👉 https://github.com/pcm720/PlayStation2-Basic-BootLoader  

This Repo's ReadMe.md has been modified from its original version to reflect this fork's documentation.  

As a result, it is recommended to familiarize yourself with this PS2BBLE documentation before continuing, even if you are familiar with PS2BBL.  

---

## Configuration Model (Read This First)

PS2BBL reads **one primary configuration file per boot**.

Some storage devices (HDD, MX4SIO, MMCE, XFROM) may not exist at boot time and can only be enabled *after* the initial configuration is parsed.  
These devices **cannot provide the CONFIG.INI used for the same boot pass**.

Configuration sources therefore fall into two categories:

1. **Always-available sources** (checked immediately)
2. **Conditionally-available sources** (checked only after device initialization)

The search order below reflects this behavior explicitly.

---

## Configuration File Search Order  
**(Activated sources only — first match wins)**

### Phase 1 — Always available

1. `CONFIG.INI` in the current working directory  
2. `mc1:/SYS-CONF/PS2BBL.INI`  
3. `mc0:/SYS-CONF/PS2BBL.INI`  
4. `mc?:/SYS-CONF/PSXBBL.INI` *(PSX builds only)*  

### Phase 2 — Conditionally available

5. `mmce1:/PS2BBL/PS2BBL.INI`  
6. `mmce0:/PS2BBL/PS2BBL.INI`  
   *(after MMCE driver initialization)*  

7. `xfrom:/PS2BBL/CONFIG.INI`  
   *(after XFROM is enabled)*  

8. `hdd0:__sysconf:pfs:/PS2BBL/CONFIG.INI`  
   *(after HDD stack initialization)*  

9. `massX:/PS2BBL/CONFIG.INI`  
   *(after MX4SIO initialization)*  

10. `mass:/PS2BBL/CONFIG.INI`

---

## Built-In Defaults (No Config Found)

If no configuration file is located, PS2BBL uses internal defaults.

### Button → Launch Mapping

**AUTO**
- `mc?:/BOOT/BOOT.ELF`
- `mc?:/BOOT/BOOT2.ELF`
- `mass:/RESCUE.ELF`

**SELECT**
- `mass:/PS2BBL/L2[1].ELF`
- `mass:/PS2BBL/L2[2].ELF`
- `mass:/PS2BBL/L2[3].ELF`

**L3**
- `mass:/PS2BBL/R2[1].ELF`
- `mass:/PS2BBL/R2[2].ELF`
- `mass:/PS2BBL/R2[3].ELF`

**R3**
- `mc?:/OPL/OPNPS2LD.ELF`
- `mc?:/APPS/OPNPS2LD/ELF`
- `mass:/PS2BBL/OPNPS2LD.ELF`

**START**
- `mass:/RESCUE.ELF`
- `mc?:/BOOT/BOOT2.ELF`
- `mc?:/APPS/ULE.ELF`

---

## Runtime Configuration Keys

These keys are parsed **only from the primary CONFIG.INI**.

They activate devices *after* configuration parsing has completed.

### Runtime Device Enablement

- `HDD_ENABLE=1`  
  Enables the HDD stack after boot  
  *(requires `HDD_RUNTIME=1` build)*

- `MX4SIO_ENABLE=1`  
  Loads `mx4sio_bd.irx` and enables `massX:`  
  *(requires `MX4SIO` or `MX4SIO_RUNTIME`)*

- `MMCE_ENABLE=1`  
  Loads `mmceman.irx` and enables `mmce0:/mmce1:`  
  *(requires `MMCE` or `MMCE_RUNTIME`)*

- `XFROM_ENABLE=1`  
  Loads `xfromman.irx` and enables `xfrom:`  
  *(requires `XFROM` or `XFROM_RUNTIME`)*

> ⚠️ These devices **cannot supply the current boot’s CONFIG.INI**.  
> To use configuration files from them, chain-load PS2BBL again or perform a second config pass.

---

### Extra IRX Loading

- `LOAD_IRX_E#=<path>`  
  Loads additional IRX modules during config parsing.

This does **not** add new configuration search paths.

---

### Button Bindings

- `LK_<BUTTON>_E#=<path>`

Example:
```

LK_AUTO_E1=mc0:/BOOT/BOOT.ELF
LK_START_E1=mass:/RESCUE.ELF

```

---

### Boot Behavior Options

- `SKIP_PS2LOGO`
- `KEY_READ_WAIT_TIME`
- `EJECT_TRAY`
- `LOGO_DISPLAY`

---

## Build-Time Driver Flags (Makefile)

### Embedded (Available Immediately)

- `HDD=1`
- `MX4SIO=1`
- `MMCE=1`
- `XFROM=1`

### Runtime-Loaded (From Memory Card)

- `HDD_RUNTIME=1`
- `MX4SIO_RUNTIME=1`
- `MMCE_RUNTIME=1`
- `XFROM_RUNTIME=1`

These require the matching `*_ENABLE=1` INI key.

---

### IRX Embedding Control

- `HAS_EMBED_IRX=1`  
  Embeds USB / BDM IRXs directly into PS2BBL  
  Otherwise they are loaded externally from memory card

---

## External IRX Lookup Order

Runtime-loaded IRX modules are searched in this order:

1. `mc0:/SYS-CONF/<IRX>`
2. `mc1:/SYS-CONF/<IRX>`
3. `mc?:/SYS-CONF/<IRX>`

---

## Example CONFIG.INI

```

HDD_ENABLE=1
MX4SIO_ENABLE=1
MMCE_ENABLE=1
XFROM_ENABLE=1

LOAD_IRX_E1=mc0:/SYS-CONF/FSCK.IRX

LK_AUTO_E1=mc0:/BOOT/BOOT.ELF
LK_AUTO_E2=mc0:/BOOT/BOOT2.ELF
LK_AUTO_E3=mass:/RESCUE.ELF

KEY_READ_WAIT_TIME=5000
LOGO_DISPLAY=2

```

Required IRX files must exist under:
`mc0:/SYS-CONF/` (or fallback memory-card paths).

---

## Additional CONFIG.INI Examples

### Memory-card-only boot (no runtime devices)

```
; Keep the boot fast and deterministic
HDD_ENABLE=0
MX4SIO_ENABLE=0
MMCE_ENABLE=0
XFROM_ENABLE=0

; Simple key bindings
LK_AUTO_E1=mc0:/BOOT/BOOT.ELF
LK_START_E1=mc0:/APPS/ULE.ELF

; Skip branding to reduce boot time
SKIP_PS2LOGO=1
```

### HDD runtime with embedded USB/BDM IRX

```
; Enable HDD after config parsing
HDD_ENABLE=1

; Expect additional drivers to be loaded from memory card
LOAD_IRX_E1=mc0:/SYS-CONF/ATAD.IRX
LOAD_IRX_E2=mc0:/SYS-CONF/DEV9.IRX

; Primary launcher lives on HDD
LK_AUTO_E1=hdd0:__sysconf:pfs:/PS2BBL/BOOT.ELF

; Fallback to memory card if HDD is missing
LK_AUTO_E2=mc0:/BOOT/BOOT.ELF

; Show logo but reduce wait
KEY_READ_WAIT_TIME=2000
LOGO_DISPLAY=1
```

### MX4SIO mass-storage first, memory card rescue second

```
MX4SIO_ENABLE=1

; Preferred launchers on MX4SIO
LK_AUTO_E1=massX:/PS2BBL/LOADER.ELF
LK_AUTO_E2=massX:/RESCUE.ELF

; Rescue path if MX4SIO is absent or fails
LK_AUTO_E3=mc0:/BOOT/BOOT2.ELF

; Keep tray closed and use a longer key-read window
EJECT_TRAY=0
KEY_READ_WAIT_TIME=8000
```

---

## Known Issues

If you find one — please report it 🙂

---

## Credits

- @SP193 — OSD initialization libraries  
- asmblur — SDK support and encouragement  
- @uyjulian, @fjtrujy — continuous assistance  
- @israpps (El_isra) — original PS2BBL creator  
- @pcm720 — reviews and contributions  

---

## Build & Usage Notes

- Requires a working **ps2sdk** toolchain in `PATH`
- Typical build:  
  `make`  
  *(add `HDD=1` or `*_RUNTIME=1` flags as needed)*
- `mk_kelf.sh` generates KELF variants
- External IRX must reside in `mc?:/SYS-CONF/` when not embedded

## Static Analysis (Codacy / cppcheck)

- Run `scripts/cppcheck.sh` locally or in Codacy. It invokes `cppcheck` with `--library=std.cfg --check-config --enable=all --std=c99 --inline-suppr --suppress=missingIncludeSystem` and includes:
  - `$PS2SDK/ee/include`, `$PS2SDK/iop/include`, `$PS2SDK/common/include` (defaulting to `/usr/local/ps2dev/ps2sdk`)
  - Project headers under `include/`
  - Analysis-only PS2SDK shims under `tools/codacy_shims/ps2sdk/` to avoid “missing include” findings when the real SDK is unavailable
