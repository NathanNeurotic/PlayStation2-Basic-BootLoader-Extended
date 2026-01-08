<br />
<p align="center">
  <a href="https://github.com/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended">
    <img width="1536" height="394" alt="PS2BBLE Logo" src="https://github.com/user-attachments/assets/6c977cdf-6d8f-47ad-961b-9b1d5f4577f7" />
  </a>
</p>

<p align="center">
  A flexible BootLoader for PlayStation 2(TM) and PSX-DESR
</p>

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/8a5773aa30cf4aca97550ec8053908d6)](
https://app.codacy.com/gh/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/dashboard
)
[![CI](https://github.com/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/actions/workflows/cl.yml/badge.svg?branch=main)](
https://github.com/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/actions/workflows/cl.yml
)
[![GitHub all releases](https://img.shields.io/github/downloads/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/total?logo=GitHub&logoColor=white)](https://github.com/NathanNeurotic/PlayStation2-Basic-BootLoader-Extended/releases/)


---

## Documentation

Full original documentation is available on GitHub Pages:  
* https://israpps.github.io/PlayStation2-Basic-BootLoader/  

This Repo is Forked from:  

* https://github.com/pcm720/PlayStation2-Basic-BootLoader  

This Repo's ReadMe.md has been modified from its original version to reflect this fork's documentation.  

As a result, it is recommended to familiarize yourself with this PS2BBLE documentation before continuing, even if you are familiar with PS2BBL.  

---

# PS2BBLE (PlayStation 2 Basic BootLoader - Extended)

A deterministic, configuration-driven bootloader for **PlayStation 2(TM)** (and supported builds for **PSX-DESR**) that:

- performs early system initialization (OSD facilities, core module loading, CDVD certification, etc.),
- can boot **discs** (PS1 / PS2 / DVD-Vide) via built-in commands,
- launches **ELF executables** from multiple storage devices using simple button bindings,
- supports **embedded** and/or **runtime-loaded** IRX driver stacks depending on the build.

PS2BBLE is designed to be **recoverable**: if your setup breaks but PS2BBLE still launches, you can enter an **Emergency Mode** and repeatedly attempt a known rescue ELF from USB.

Forked from @pcm720's PS2BBL fork of @israpps's PS2BBL, the parent projects of PS2BBLE, lightweight yet powerful bootloaders for **PlayStation 2(TM)** and **PSX-DESR** systems.  
All intended performance and functions were targeted to preserve the existing PS2BBL features and performance, while enhancing and adding to the project as a whole.
Both perform early system initialization and launches ELF executables from multiple storage backends, supporting both **embedded** and **runtime-loaded** drivers.
Their boot processes are deterministic, with a clearly defined configuration search order and strict separation between **boot-time** and **runtime-enabled** devices.


---

## Table of contents

1. [What PS2BBLE is](#what-ps2bble-is)  
2. [Ways to use it](#ways-to-use-it)  
3. [Safety and recovery](#safety-and-recovery)  
4. [What you need](#what-you-need)  
5. [Quick start](#quick-start)  
6. [How boot works](#how-boot-works)  
7. [Config file search order](#config-file-search-order)  
8. [Configuration keys](#configuration-keys)  
9. [Launch keys (button bindings)](#launch-keys-button-bindings)  
10. [Internal commands](#internal-commands)  
11. [Devices and path formats](#devices-and-path-formats)  
12. [Drivers: embedded vs runtime](#drivers-embedded-vs-runtime)  
13. [Example CONFIG.INI files](#example-configini-files)  
14. [Troubleshooting (panic-friendly)](#troubleshooting-panic-friendly)  
15. [Compatibility notes](#compatibility-notes)  
16. [Differences vs FreeMcBoot (practical)](#differences-vs-freemcboot-practical)  
17. [Build notes](#build-notes)  
18. [Credits and license](#credits-and-license)  

---

## What PS2BBLE is

PS2BBLE is a bootloader. It reads **one configuration file per boot**, waits for controller input for a configured time window, and then runs the first valid launch entry for the pressed button (or runs **AUTO** if no button is pressed).

It does **not** merge configs, scan directories, or "guess" paths. Everything is driven by your config and the build's enabled features.

---

## Ways to use it

PS2BBL-family bootloaders are commonly used in these modes:

### 1) Bare ELF (most common)
You boot `PS2BBLE.ELF` like any homebrew ELF from an existing entry point (uLaunchELF / wLaunchELF, OPL, FreeDVDBoot, modchip DEV1/DEV2, OpenTuna/FreeHDBoot flows, etc.).

### 2) Installed boot entry (when provided by a release/build)
Some distributions provide tooling or packaging to install the bootloader as an encrypted/system boot entry so it auto-starts without you manually launching the ELF each time. Depending on the platform and setup, this can be done via:
- a memory-card "system update / encrypted executable" style install, and/or
- a HDD-based boot path (where applicable to the environment and release packaging).
- KELFBinder by @israpps

> Whether a given PS2BBLE release supports one-click install/auto-boot is determined by the **release** you ship (and its target hardware), not by this README alone.

---

## Safety and recovery

### Emergency Mode (built-in recovery)
If PS2BBLE still runs but your config/app paths are broken, hold:

**R1 + START**

PS2BBLE enters an endless loop and repeatedly tries:

```
mass:/RESCUE.ELF
```
mass is recommended to be a fat32 32kb clustered USB of any size equal to or less than 2TB

### Recommended recovery safety net (not required)
It is *not required* for normal operation, but it's strongly recommended to keep **some** known-good rescue app available so you can recover if things go wrong.

Typical choice:
- uLaunchELF / wLaunchELF copied as: `mass:/RESCUE.ELF`

---

## What you need

### One way to launch PS2BBLE at least once
Any existing homebrew entry point works, for example:
- uLaunchELF / wLaunchELF
- OPL (Open PS2 Loader)
- FreeDVDBoot (disc entry point)
- modchip DEV1/DEV2 and similar
- OpenTuna / FreeHDBoot style entry points
- environment-specific loaders (examples seen in the ecosystem include YADE/MMCE-related flows, ATA/HDD loader flows, etc.)

*(You do not need all of these. You just need **one** that works on your console.)*

### USB drive (recommended)
For USB (`mass:`) usage:
- Prefer **FAT32**
- Prefer **MBR** partitioning (not GPT) for maximum compatibility
- 32 KB cluster size is a common "safe default" for homebrew workflows
- For windows users, it is recommended to perform the format with [rufus](https://rufus.ie)

### Memory card (recommended)
- A **MagicGate-capable** memory card is strongly recommended for broad compatibility
- If you use MMCE device(s), follow the device's [expected setup](https://sd2psxtd.github.io)/[official setup](https://www.8bitmods.wiki/memcard-pro2) for your target build/variant

### A way to edit a text file
- Any PC text editor works, **or**
- uLaunchELF / wLaunchELF's built-in text editor can be used to create/edit the INI directly on-device

---

## Quick start

This is the simplest "beginner-safe" setup that works across the widest range of situations.

### Step 1 - Copy PS2BBLE somewhere you can launch it
Place `PS2BBLE.ELF` on a device you can access from your entry point (USB or memory card).

### Step 2 - Create the config file on a reliable location
PS2BBLE searches memory cards early and reliably. Create:

- `mc0:/SYS-CONF/PS2BBL.INI`

Optionally (recommended for "global fallback"), duplicate the same file to or choose instead:

- `mc1:/SYS-CONF/PS2BBL.INI`

> **Beginner rule (global fallback):** PS2BBLE searches `mc1` then `mc0`. If you want a "works no matter which slot you use" feel, keep the same config on **both** cards or update the INI to mc?:/ instead of mc0:/ or mc1:/

### Step 3 - Paste a minimal safe config
This config keeps the boot window short and uses only always-available paths.

```ini
# Minimal safe PS2BBLE config
KEY_READ_WAIT_TIME=500
LOGO_DISPLAY=2

# AUTO fallback chain (tries in order)
LK_AUTO_E1=mc?:/BOOT/BOOT.ELF
LK_AUTO_E2=mc?:/BOOT/BOOT2.ELF
```

*(This minimal example intentionally does not require a USB rescue file. Add recovery later if you want it.)*

### Step 4 - Boot PS2BBLE
- If you press nothing: after **500 ms**, it runs the first found AUTO entry.
- If you want a recovery backdoor, see the next section and Emergency Mode.

---

## How boot works

At a high level:

1. Initialize system basics and drivers (what's available depends on the build).
2. Search for a configuration file in a fixed order.
3. Parse known keys (`KEY=VALUE`) and store launch mappings.
4. Wait for input for `KEY_READ_WAIT_TIME`.
5. Launch the first valid entry for the pressed button; otherwise run AUTO.

Important practical point:
- Enabling some devices may happen after config parsing depending on build/driver availability, so a device may be usable for launching apps even if it couldn't provide the config for that boot pass.

---

## Config file search order

PS2BBLE searches for one config file in the following order (first readable match wins):

1. `CONFIG.INI` (current working directory)
2. `mc1:/SYS-CONF/PS2BBL.INI`
3. `mc0:/SYS-CONF/PS2BBL.INI`
4. `mc?:/SYS-CONF/PSXBBL.INI` *(PSX / DESR builds)*
5. `mmce1:/PS2BBL/PS2BBL.INI` *(when MMCE is enabled & available)*
6. `mmce0:/PS2BBL/PS2BBL.INI` *(when MMCE is enabled & available)*
7. `xfrom:/PS2BBL/CONFIG.INI` *(when XFROM is enabled & available)*
8. `hdd0:__sysconf:pfs:/PS2BBL/CONFIG.INI` *(when HDD is enabled & available)*
9. `massX:/PS2BBL/CONFIG.INI` *(MX4SIO mapping when enabled & available)*
10. `mass:/PS2BBL/CONFIG.INI`

### Recommended default config placement (beginner-safe)
- PS2: keep your primary config at `mc1:/SYS-CONF/PS2BBL.INI` and optionally duplicate to `mc0:/SYS-CONF/PS2BBL.INI`; or just use mc?:/SYS-CONF/PS2BBL.INI
- PSX-DESR builds: place a PSX-specific config at `mc?:/SYS-CONF/PSXBBL.INI` when you want PSX-targeted behavior.

---

## Configuration keys

### Boot behavior
- `SKIP_PS2LOGO`  
  Controls PS2 disc boot logo behavior (also affected by `$CDVD_NO_PS2LOGO`).
- `KEY_READ_WAIT_TIME`  
  Milliseconds to wait for button input before running AUTO.
- `OSDHISTORY_READ`  
  Enables OSD history reading (used by some builds for behavior/cosmetic decisions).
- `EJECT_TRAY`  
  Eject tray the first time an empty tray is detected.
- `LOGO_DISPLAY`  
  - `0`: no logo/info  
  - `1`: console info only  
  - `2`: logo + console info  

### Device enablement (when supported by your build)
- `HDD_ENABLE=1`
- `MX4SIO_ENABLE=1`
- `MMCE_ENABLE=1`
- `XFROM_ENABLE=1`

### Extra IRX loading
- `LOAD_IRX_E#=<path>`  
  Loads additional IRX modules during startup/parse time (useful for some setups).
  - This does not automatically guarantee a device becomes a config source for the same boot pass.

---

## Launch keys (button bindings)

Launch keys map controller buttons to either:
- an ELF path, or
- an internal command (starts with `$`).

### Syntax
```text
LK_<BUTTON>_E#
```

- `<BUTTON>`:  
  `AUTO, SELECT, L3, R3, START, UP, RIGHT, DOWN, LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE`
- `E#`: entry index (use `E1`..`E3` for simple, reliable setups)

PS2BBLE checks entries in order (`E1` -> `E2` -> `E3`) until one works.

---

## Internal commands

You can bind commands instead of ELF paths:

### Disc boot commands
- `$CDVD`  
  Boot disc, respecting the config's logo preference.
- `$CDVD_NO_PS2LOGO`  
  Boot disc forcing "skip PS2LOGO".

Example:
```ini
LK_CROSS_E1=$CDVD
LK_TRIANGLE_E1=$CDVD_NO_PS2LOGO
```

### Utility/system commands (when supported by your build)
- `$CREDITS` - show credits/build info
- `$OSDSYS` - run OSDSYS with arguments intended to skip update flows and land in MC browser  
  *(On PSX-DESR, OSDSYS menus differ; this may be useless there.)*
- `$RUNKELF:<KELFPATH>` - run a KELF at `<KELFPATH>`
- `$HDDCHECKER` - HDD integrity/status checks (only when HDD support is enabled)

---

## Devices and path formats

Common executable path formats:

- `mc0:/...` - Memory Card slot 1  
- `mc1:/...` - Memory Card slot 2  
- `mc?:/...` - pseudo-device that searches both MC slots  
- `mass:/...` - USB storage mapped by USBMASS  
- `massX:/...` - MX4SIO SD mapping (when enabled)  
- `rom0:...` - console ROM executables (e.g., `rom0:TESTMODE`)  
- `hdd0:PARTITION:pfs:/path/to.elf` - internal HDD PFS path  

Example:
```ini
LK_SELECT_E1=rom0:TESTMODE
LK_AUTO_E1=hdd0:__sysconf:pfs:/PS2BBL/BOOT.ELF
```

---

## Drivers: embedded vs runtime

PS2BBLE can be built with driver stacks that are:

### Embedded
Drivers are built into the ELF, reducing dependency on external IRX files (helpful if storage contents are lost or corrupted).

### Runtime-loaded
Drivers are loaded from storage at boot. In these builds, keep required IRX files available on the memory card.

Typical external IRX lookup order:
1. `mc0:/SYS-CONF/<IRX>`
2. `mc1:/SYS-CONF/<IRX>`
3. `mc?:/SYS-CONF/<IRX>`

---

## Example CONFIG.INI files

### 1) Minimal safe (MC-only, fast boot)
```ini
KEY_READ_WAIT_TIME=500
LOGO_DISPLAY=2

LK_AUTO_E1=mc?:/BOOT/BOOT.ELF
LK_AUTO_E2=mc?:/BOOT/BOOT2.ELF
```

### 2) Add a recovery safety net (optional)
```ini
KEY_READ_WAIT_TIME=500
LOGO_DISPLAY=2

LK_AUTO_E1=mc?:/BOOT/BOOT.ELF
LK_AUTO_E2=mc?:/BOOT/BOOT2.ELF
LK_AUTO_E3=mass:/RESCUE.ELF
```

And keep a rescue app at:
```
mass:/RESCUE.ELF
```

### 3) Disc boot buttons
```ini
KEY_READ_WAIT_TIME=500
LOGO_DISPLAY=2
SKIP_PS2LOGO=0

LK_AUTO_E1=mc?:/BOOT/BOOT.ELF
LK_CROSS_E1=$CDVD
LK_TRIANGLE_E1=$CDVD_NO_PS2LOGO
```

### 4) Advanced example (apps + commands + IRX load)
```ini
# Boot behavior
SKIP_PS2LOGO=0
EJECT_TRAY=1
OSDHISTORY_READ=1
KEY_READ_WAIT_TIME=4000
LOGO_DISPLAY=2

# Optional: load extra IRX
LOAD_IRX_E1=mass:/PS2BBL/EXAMPLE.IRX

# AUTO chain
LK_AUTO_E1=mc?:/APPS/OPNPS2LD.ELF
LK_AUTO_E2=mc?:/OPL/OPNPS2LD.ELF
LK_AUTO_E3=mass:/OPNPS2LD.ELF

# System & disc
LK_SELECT_E1=rom0:TESTMODE
LK_START_E1=$OSDSYS
LK_CROSS_E1=$CDVD
LK_TRIANGLE_E1=$CDVD_NO_PS2LOGO
```

---

## Troubleshooting (panic-friendly)

### If nothing launches
1. **Emergency Mode:** hold **R1 + START**  
   If you have a rescue ELF at `mass:/RESCUE.ELF`, PS2BBLE will repeatedly attempt it.
2. **Put your config on memory card:**  
   `mc1:/SYS-CONF/PS2BBL.INI` (optionally duplicate to `mc0:`).
3. **Check your paths carefully:**  
   A single typo will cause an entry to fail and fall through to the next entry.
4. **If using runtime drivers:**  
   Ensure required IRX files exist under `mc0:/SYS-CONF/`.

### If MMCE/MX4SIO/XFROM/HDD paths don't work
Usually means one of:
- your build does not include that feature,
- the device isn't enabled/initialized successfully,
- required IRX files are missing (runtime builds).

Start with a working MC-only config, then add one device at a time.

---

## Compatibility notes

Upstream PS2BBL documentation reports broad compatibility across many retail PS2 models and PSX-DESR variants, with special notes for:
- very early Japanese models requiring kernel patch updates,
- some late SCPH-900xx situations where "system update" constraints can affect auto-boot style installs.

If you publish releases targeting specific hardware/variants (PS2 retail vs PSX-DESR vs Namco System 2x6), include a release note that states:
- the target,
- supported devices,
- and what install/auto-boot modes are included (if any).

---

## Differences vs FreeMcBoot (practical)

PS2BBL-family bootloaders are commonly used as an alternative to FreeMcBoot because they:
- emphasize deterministic config-driven boot behavior,
- provide disc-boot commands and emergency recovery loops,
- offer builds with embedded drivers (reducing dependency on external files),
- can be friendlier to some modchip environments (depending on setup/build) by avoiding certain OSDSYS patching approaches used elsewhere.

Both ecosystems are valuable; choose what best matches your hardware and goals.

---

## Build notes

Building requires a working **ps2sdk** toolchain in `PATH`.

Typical build:
```sh
make
```

cl commonly builds multiple variants by enumerating feature flags (embedded vs runtime drivers, device support, chainload modes, etc.).

---

## Credits and license

- OSD initialization libraries and related work: **@SP193**
- Original PS2BBL creator: **@israpps (El_isra)**
- Fork lineage and contributions: **@pcm720**, plus community contributors
- License: **GNU General Public License v3.0** (see `LICENSE`)
