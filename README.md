# PS2BBLE (PlayStation 2 Basic BootLoader — Extended)

A small bootloader that reads **one configuration file per boot** and launches ELF (**executable**) programs based on button bindings or an AUTO (default) fallback. It searches known config locations in a fixed order and uses the **first** readable file it finds, when that file is reachable on the currently available devices.【F:src/main.c†L33-L45】【F:src/main.c†L632-L653】

**Documentation accuracy:** This README reflects the current source code and cl workflow configuration. If code changes, update this document to match the new behavior.【F:src/main.c†L33-L965】【F:.github/workflows/cl.yml†L1-L211】

---

## Table of contents

1. [What This Project Is](#what-this-project-is)
2. [What It’s Not](#what-its-not)
3. [Safety & Risk](#safety--risk)
4. [Glossary](#glossary)
5. [How Boot Works (Simplified)](#how-boot-works-simplified)
6. [What PS2BBLE Does During Boot (Verified Flow)](#what-ps2bble-does-during-boot-verified-flow)
7. [Storage/Devices (Verified)](#storagedevices-verified)
8. [Quick Start (I just want to use it)](#quick-start-i-just-want-to-use-it)
9. [Step-by-Step Setup](#step-by-step-setup)
10. [Configuration](#configuration)
11. [Advanced: Drivers (Embedded vs Runtime)](#advanced-drivers-embedded-vs-runtime)
12. [Build (Optional / Advanced)](#build-optional--advanced)
13. [Troubleshooting (Panic-Friendly Checklist)](#troubleshooting-panic-friendly-checklist)
14. [FAQ](#faq)
15. [Credits / Disclaimer / Legal](#credits--disclaimer--legal)

---

## What This Project Is

PS2BBLE is a bootloader that:

- **Searches for one config file** at boot, using a fixed path order, and uses the first match **that can be read on the devices that are available at that time**.【F:src/main.c†L33-L45】【F:src/main.c†L632-L653】
- **Parses known INI (configuration) keys** and applies boot options and button bindings.【F:src/main.c†L679-L767】
- **Launches ELF executables** when a bound button is pressed, or uses AUTO after the wait timeout.【F:src/main.c†L921-L965】

---

## What It’s Not

- It is **not** confirmed in this fork to provide PSX-DESR-specific behavior. The code supports a PSX build flag, but DESR-specific guarantees are not proven here.【F:Makefile†L30-L31】【F:src/main.c†L229-L231】
- It does **not** guarantee device availability at boot beyond what the build and runtime IRX loading allows. If a device is missing or IRX files are absent, behavior depends on runtime conditions.【F:src/main.c†L485-L629】【F:src/main.c†L1308-L1363】

---

## Safety & Risk

PS2BBLE loads and runs ELFs from your configured paths. This is powerful, but it also means:

- A wrong path or missing file results in **“not found”** behavior rather than a crash loop, and it will continue to other entries or AUTO as coded.【F:src/main.c†L921-L965】
- **Emergency mode** exists: holding **R1 + START** triggers a loop that repeatedly tries `mass:/RESCUE.ELF`.【F:src/main.c†L612-L616】【F:src/main.c†L982-L993】

**WARNING:** If your USB drive or memory card paths are wrong, the bootloader won’t find your ELFs. Keep a known-good `mass:/RESCUE.ELF` available if you rely on emergency mode.【F:src/main.c†L982-L993】

---

## Glossary

- **ELF**: Executable file format used for PS2 homebrew (e.g., `BOOT.ELF`).
- **IOP**: Input/Output Processor (the PS2 coprocessor that runs IRX modules).
- **IRX**: IOP module (driver) loaded by the bootloader (e.g., USB or storage drivers).【F:src/main.c†L1149-L1193】
- **INI**: Simple `NAME=VALUE` text configuration file parsed by the bootloader.【F:src/util.c†L65-L115】
- **mc0 / mc1**: Memory card slots 0 and 1.
- **mass:** USB storage device path.
- **massX:** Placeholder that is resolved to `mass0:`–`mass4:` when MX4SIO is in use.【F:src/main.c†L1120-L1124】
- **mmce0 / mmce1**: Memory card emulation devices (when MMCE is enabled).【F:src/main.c†L315-L360】
- **xfrom:** External flash device path (when XFROM is enabled).【F:src/main.c†L367-L389】

---

## How Boot Works (Simplified)

1. Initialize core systems and drivers.
2. Search for a configuration file in the fixed order and stop at the first match **that can be read on available devices**.【F:src/main.c†L33-L45】【F:src/main.c†L632-L653】
3. Parse known keys and apply settings, including button bindings.【F:src/main.c†L679-L767】
4. Wait for button input for the configured timeout, then run bound ELFs or AUTO.【F:src/main.c†L921-L965】

---

## What PS2BBLE Does During Boot (Verified Flow)

This is the actual flow from the main program:

- Initializes IOP, file I/O, and core modules.
- Loads USB drivers (embedded or external depending on build).【F:src/main.c†L1149-L1193】
- Optionally enables additional device drivers if compiled in and available.【F:src/main.c†L485-L520】
- Loads OSD data, applies config, and sets video output parameters.【F:src/main.c†L542-L600】
- Searches for `CONFIG.INI` in the fixed path order and parses recognized keys if the file is reachable on available devices.【F:src/main.c†L33-L45】【F:src/main.c†L632-L767】
- Waits for button input, then loads the first existing ELF for that button; otherwise runs AUTO.【F:src/main.c†L921-L965】

**NOTE:** Some devices may be enabled before config parsing depending on build flags and runtime driver availability, which affects whether those devices can supply the config file on that boot.【F:src/main.c†L485-L629】【F:src/main.c†L1308-L1363】

---

## Storage/Devices (Verified)

These storage backends exist in code and are used as config sources or execution paths when the device and its drivers are available:

- **CWD**: `CONFIG.INI` in the current working directory.【F:src/main.c†L33-L45】
- **Memory cards**: `mc0:` and `mc1:` (including `mc?:` auto-slot selection).【F:src/main.c†L33-L45】【F:src/main.c†L1086-L1094】
- **USB mass storage**: `mass:` (always checked) and `massX:` (MX4SIO mode).【F:src/main.c†L33-L45】【F:src/main.c†L1120-L1124】
- **MMCE**: `mmce0:` / `mmce1:` when enabled by build/runtime.【F:src/main.c†L315-L360】
- **XFROM**: `xfrom:` when enabled by build/runtime.【F:src/main.c†L367-L389】
- **HDD**: `hdd0:__sysconf:pfs:/...` when HDD support is available.【F:src/main.c†L33-L45】【F:src/main.c†L1370-L1433】

---

## Quick Start (I just want to use it)

This path assumes you already have a built PS2BBLE ELF.

1. **Copy PS2BBLE ELF** to a location you can boot (e.g., memory card or USB).  
   **What you should see:** Your launcher or file browser shows the PS2BBLE ELF where you copied it.
2. **Create a config file** in one of the default locations, for example:
   - `mc0:/SYS-CONF/PS2BBL.INI`
   - `mass:/PS2BBL/CONFIG.INI`
   **What you should see:** The file exists in that exact path (case-sensitive on some tools).
3. **Add at least one AUTO entry** (example below).  
   **What you should see:** The config file contains lines like `LK_AUTO_E1=...`.
4. Boot PS2BBLE and let AUTO run after the timeout, or press a bound button.  
   **What you should see:** If the ELF exists, you should see a “Loading <path>” message before it launches.【F:src/main.c†L921-L965】

**Recovery if it fails:** Hold **R1 + START** to enter emergency mode and attempt `mass:/RESCUE.ELF` repeatedly.【F:src/main.c†L612-L616】【F:src/main.c†L982-L993】

Minimal example (AUTO only):

```
LK_AUTO_E1=mc0:/BOOT/BOOT.ELF
KEY_READ_WAIT_TIME=5000
LOGO_DISPLAY=2
```

**TIP:** Keep a known-good rescue ELF at `mass:/RESCUE.ELF` so you can recover using **R1 + START** if needed.【F:src/main.c†L982-L993】

---

## Step-by-Step Setup

1. **Pick a config location** from the fixed search order. For example:
   - `mc0:/SYS-CONF/PS2BBL.INI`
   - `mass:/PS2BBL/CONFIG.INI`【F:src/main.c†L33-L45】
   **What you should see:** The file is present in the exact path you chose.
2. **Create the config file** using `NAME=VALUE` entries (see [Configuration](#configuration)).【F:src/util.c†L65-L115】  
   **What you should see:** Each line follows `KEY=VALUE` with no extra spaces around `=`.
3. **Add button bindings** using `LK_<BUTTON>_E#` keys (e.g., `LK_START_E1`).【F:src/main.c†L743-L760】【F:src/common.c†L17-L35】  
   **What you should see:** At least one binding points to a real ELF path.
4. **Boot PS2BBLE**. If you press a bound button during the wait period, its ELF is launched; otherwise AUTO runs after the timeout.【F:src/main.c†L921-L965】  
   **What you should see:** “Loading <path>” for a found ELF; if not found, it will continue to other entries or AUTO.【F:src/main.c†L921-L965】

**Recovery if it fails:** Move the config file to a more reliable device (e.g., `mc0:`), or add a rescue entry on USB so emergency mode can recover.【F:src/main.c†L632-L653】【F:src/main.c†L982-L993】

---

## Configuration

### Config search order (first match wins)

The bootloader searches **in this exact order** and uses the first readable file it can access on available devices:

1. `CONFIG.INI` (current working directory)
2. `mc1:/SYS-CONF/PS2BBL.INI`
3. `mc0:/SYS-CONF/PS2BBL.INI`
4. `mc?:/SYS-CONF/PSXBBL.INI`
5. `mmce1:/PS2BBL/PS2BBL.INI`
6. `mmce0:/PS2BBL/PS2BBL.INI`
7. `xfrom:/PS2BBL/CONFIG.INI`
8. `hdd0:__sysconf:pfs:/PS2BBL/CONFIG.INI`
9. `massX:/PS2BBL/CONFIG.INI`
10. `mass:/PS2BBL/CONFIG.INI`【F:src/main.c†L33-L45】

**NOTE:** Some devices are only available if built with specific flags and if their IRX drivers load successfully. If a device is unavailable, its paths won’t be usable even if they appear in the list.【F:src/main.c†L485-L629】【F:src/main.c†L1308-L1363】

### Supported INI keys (verified)

#### Boot behavior
- `SKIP_PS2LOGO`
- `KEY_READ_WAIT_TIME`
- `EJECT_TRAY`
- `LOGO_DISPLAY`
- `OSDHISTORY_READ`【F:src/main.c†L679-L716】

#### Device enablement (conditional)
- `HDD_ENABLE`
- `MX4SIO_ENABLE`
- `MMCE_ENABLE`
- `XFROM_ENABLE`【F:src/main.c†L718-L740】

#### Extra IRX loading
- `LOAD_IRX_E#=<path>` (loads an additional IRX at parse time)【F:src/main.c†L683-L700】

#### Button bindings
- `LK_<BUTTON>_E#=<path>` (e.g., `LK_START_E1=mass:/RESCUE.ELF`)【F:src/main.c†L743-L760】

**Valid button names:**
`AUTO, SELECT, L3, R3, START, UP, RIGHT, DOWN, LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE`.【F:src/common.c†L17-L35】

### Example config

```
OSDHISTORY_READ=1
SKIP_PS2LOGO=0
KEY_READ_WAIT_TIME=5000
LOGO_DISPLAY=2

LK_AUTO_E1=mc0:/BOOT/BOOT.ELF
LK_START_E1=mass:/RESCUE.ELF
```

---

## Advanced: Drivers (Embedded vs Runtime)

Some features are **build-flag dependent**. Use conditional language when documenting them:

- **USB drivers**
  - `HAS_EMBED_IRX=1` embeds USB/BDM IRX drivers.
  - Otherwise, external IRX files are loaded from `mc?:/SYS-CONF/` at boot.【F:Makefile†L313-L317】【F:src/main.c†L1149-L1193】

- **HDD support**
  - `HDD=1`: embedded IRX stack, enables HDD config path when usable.【F:Makefile†L321-L328】【F:src/main.c†L1370-L1424】
  - `HDD_RUNTIME=1`: loads external IRX stack using the IRX lookup order (see below).【F:Makefile†L331-L337】【F:src/main.c†L1308-L1363】

- **MX4SIO / MMCE / XFROM**
  - `MX4SIO=1` or `MX4SIO_RUNTIME=1` (MX4SIO_BD IRX).【F:Makefile†L208-L214】【F:src/main.c†L275-L310】
  - `MMCE=1` or `MMCE_RUNTIME=1` (MMCEMAN IRX).【F:Makefile†L218-L220】【F:src/main.c†L315-L360】
  - `XFROM=1` or `XFROM_RUNTIME=1` (XFROMMAN IRX loaded externally).【F:src/main.c†L367-L389】

### External IRX lookup order

When external IRX loading is used, this is the search order:

1. `mc0:/SYS-CONF/<IRX>`
2. `mc1:/SYS-CONF/<IRX>`
3. `mc?:/SYS-CONF/<IRX>`【F:src/main.c†L1024-L1031】

---

## Build (Optional / Advanced)

cl builds multiple variants by running `make list-variants` and building each entry in the matrix. If you build locally, your results can differ based on your flags and available IRX files.【F:.github/workflows/cl.yml†L27-L101】

**NOTE:** Build flags enable or disable runtime features, and some features require external IRX files to be present at boot.【F:Makefile†L313-L337】【F:src/main.c†L1308-L1363】

---

## Troubleshooting (Panic-Friendly Checklist)

If nothing boots or you’re stuck:

1. **Did PS2BBLE find your config file?**
   - It only uses the first readable file in the fixed search order.【F:src/main.c†L33-L45】【F:src/main.c†L632-L653】
2. **Are your paths correct?**
   - If a path is wrong, it prints “not found” and moves on.【F:src/main.c†L921-L965】
3. **Do you have a rescue ELF?**
   - Hold **R1 + START** to try `mass:/RESCUE.ELF` in a loop.【F:src/main.c†L612-L616】【F:src/main.c†L982-L993】
4. **Are external IRX files present (if needed)?**
   - External IRX load order is `mc0 → mc1 → mc?`.【F:src/main.c†L1024-L1031】
5. **Is the device actually available?**
   - Some devices are only usable when compiled in or when runtime IRX loading succeeds.【F:src/main.c†L485-L629】【F:src/main.c†L1308-L1363】

**WARNING:** If your config file is on a device that isn’t initialized (or its IRX files are missing), PS2BBLE will skip it and fall back to defaults or other sources. Keep a backup config on memory card or USB.【F:src/main.c†L632-L653】【F:src/main.c†L803-L812】

---

## FAQ

**Q: Does it always boot from HDD/MMCE/MX4SIO/XFROM?**  
A: It **can**, but only when built with the correct flags and when the required IRX drivers load successfully. Device availability at boot is conditional.【F:Makefile†L321-L337】【F:src/main.c†L275-L389】【F:src/main.c†L1308-L1363】

**Q: Why didn’t it read my config file?**  
A: It searches in a fixed order and uses only the first readable file. If that device isn’t available or the file isn’t present, it skips to the next path.【F:src/main.c†L33-L45】【F:src/main.c†L632-L653】

**Q: What are the valid button names for LK_ keys?**  
A: `AUTO, SELECT, L3, R3, START, UP, RIGHT, DOWN, LEFT, L2, R2, L1, R1, TRIANGLE, CIRCLE, CROSS, SQUARE`.【F:src/common.c†L17-L35】

**Q: Can I run special commands instead of an ELF?**  
A: Yes. Paths starting with `$` execute internal commands like `$CDVD`, `$OSDSYS`, or `$RUNKELF:`. Use with care.【F:src/main.c†L1066-L1084】

---

## Credits / Disclaimer / Legal

- This repository includes a `LICENSE` file; see it for legal terms.
- PS2BBLE is a bootloader project; compatibility details depend on build flags and device availability at runtime, and should be validated for your specific setup.【F:Makefile†L16-L45】【F:src/main.c†L485-L653】
