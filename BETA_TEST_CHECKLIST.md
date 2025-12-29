# PS2BBL Extended — Beta Tester Checklist

Comprehensive pass/fail worksheet for verifying every IRX provisioning path and device combination on real hardware. Check off every item. Record **P/F**, console model/SCPH, ROMVER, and storage details for each row.

## How to use
- Build each configuration exactly as specified (toggle Makefile flags accordingly). Note whether IRXs are **embedded in the ELF** or **loaded externally** from memory card (`mc?:/SYS-CONF/`) or other media.
- For external IRX tests, place modules at the indicated paths before boot.
- Run on real consoles (fat, slim, Tool/DEX, DESR where applicable). Note HDD presence/partitioning, MX4SIO card type, MMCE/SD wiring, and USB devices used.
- For runtime-enabled stacks, verify both the **config enable key** and the **driver load result**.

## Common external IRX paths
- `mc0:/SYS-CONF/USBD.IRX`
- `mc0:/SYS-CONF/BDM.IRX`
- `mc0:/SYS-CONF/BDMFS_FATFS.IRX`
- `mc0:/SYS-CONF/USBMASS_BD.IRX`
- `mc0:/SYS-CONF/PS2DEV9.IRX`
- `mc0:/SYS-CONF/POWEROFF.IRX`
- `mc0:/SYS-CONF/PS2ATAD.IRX`
- `mc0:/SYS-CONF/PS2HDD.IRX`
- `mc0:/SYS-CONF/PS2FS.IRX`
- `mc0:/SYS-CONF/MMCEMAN.IRX`
- `mc0:/SYS-CONF/MX4SIO_BD.IRX`
- `mc0:/SYS-CONF/XFROMMAN.IRX`
- For runtime dev9-less UDPTTY: also stage `NETMAN.IRX`, `SMAP.IRX`, `PS2IP.IRX`, `UDPTTY.IRX`.
- Use `mc1:/SYS-CONF/` duplicates where noted.

## Core USB stack (HAS_EMBED_IRX vs external)
- [ ] **Embedded USB**: `HAS_EMBED_IRX=1`, no external IRX present. Verify mass:/ mount, device hotplug, and boot of `LK_AUTO` target from USB.
- [ ] **External USB (mc0)**: `HAS_EMBED_IRX=0`, place `USBD.IRX`, `BDM.IRX`, `BDMFS_FATFS.IRX`, `USBMASS_BD.IRX` in `mc0:/SYS-CONF/`. Confirm load, mass:/ visibility, and launch path.
- [ ] **External USB (mc1 fallback)**: Same as above, files only on mc1. Confirm fallback works.

## Baseline pads/MC (no optional drivers)
- [ ] **ROM pad/MC drivers**: `HOMEBREW_IRX=0`, `HAS_EMBED_IRX=1`, all optional stacks off. Confirm pad input, MC access, logo timeout boot.
- [ ] **Homebrew pad/MC drivers embedded**: `HOMEBREW_IRX=1`, embedded IRX objects. Confirm pad/MC operation and config read.
- [ ] **Homebrew pad/MC external**: `HOMEBREW_IRX=1`, strip embedded pad IRX, stage `SIO2MAN.IRX`, `MCMAN.IRX`, `MCSERV.IRX`, `PADMAN.IRX` in `mc0:/SYS-CONF/`. Boot with/without mc0 inserted to verify failure reporting.

## HDD stack combinations (embedded vs external)
- [ ] **Embedded HDD full stack**: `HDD=1`, `HAS_EMBED_IRX=1`, DEV9 present. Confirm `hdd0:` status, `pfs0:` mount, `HDD_ENABLE` key ignored (always on), and loading ELF from `hdd0:__sysconf:pfs:/PS2BBL/CONFIG.INI`.
- [ ] **Embedded HDD without PS2FS (negative)**: Temporarily remove `ps2fs_irx` from build; expect mount failure with clear error.
- [ ] **External HDD stack (runtime)**: `HDD_RUNTIME=1`, `HDD=0`, `HAS_EMBED_IRX=0`; place `PS2DEV9.IRX`, `POWEROFF.IRX`, `PS2ATAD.IRX`, `PS2HDD.IRX`, `PS2FS.IRX` on `mc0:/SYS-CONF/`. Confirm `HDD_ENABLE=1` triggers load; verify pfs mount and unmount behavior.
- [ ] **External HDD fallback mc1**: Same as above but only mc1 populated; verify load.
- [ ] **Runtime HDD without PS2FS (negative)**: Omit `PS2FS.IRX` externally; expect controlled failure message, no crash.

## MX4SIO combinations
- [ ] **Embedded MX4SIO BD**: `MX4SIO=1`, `HAS_EMBED_IRX=1`, FILEXIO included. Confirm detection of `sdc` device, `massX:/` mount, and config search path enablement.
- [ ] **MX4SIO external (runtime)**: `MX4SIO_RUNTIME=1`, stage `MX4SIO_BD.IRX` in `mc0:/SYS-CONF/`. Test `MX4SIO_ENABLE=1` and `MX4SIO_ENABLE=0` in CONFIG.INI (ensure path disables when zero). Repeat with only mc1 populated.
- [ ] **MX4SIO + HDD embedded**: `MX4SIO=1 HDD=1 HAS_EMBED_IRX=1`. Verify both `massX:/` and `pfs0:/` simultaneously and config search ordering.
- [ ] **MX4SIO runtime + HDD runtime external**: `MX4SIO_RUNTIME=1 HDD_RUNTIME=1 HAS_EMBED_IRX=0`; all IRX on `mc0:/SYS-CONF/`. Confirm both stacks load via CONFIG enables and co-exist.
- [ ] **MX4SIO negative (no card)**: Build with `MX4SIO_RUNTIME=1`, set `MX4SIO_ENABLE=1`, boot without MX4SIO adapter; confirm graceful failure message and continued boot.

## MMCE combinations
- [ ] **Embedded MMCE**: `MMCE=1`, `HAS_EMBED_IRX=1`. Confirm `mmce0:/`/`mmce1:/` mount, config search enablement, and ELF load from SD.
- [ ] **MMCE external (runtime)**: `MMCE_RUNTIME=1`, place `MMCEMAN.IRX` on `mc0:/SYS-CONF/`. Test `MMCE_ENABLE=1/0`, repeat with only mc1 populated.
- [ ] **MMCE + HDD embedded**: `MMCE=1 HDD=1 HAS_EMBED_IRX=1`; verify simultaneous `mmce` + `pfs` access.
- [ ] **MMCE runtime + HDD runtime external**: `MMCE_RUNTIME=1 HDD_RUNTIME=1 HAS_EMBED_IRX=0`; confirm both load via CONFIG enables.
- [ ] **MMCE negative (no SD)**: Runtime build with `MMCE_ENABLE=1`, no SD inserted; confirm clean error path and no hang.

## XFROM combinations
- [ ] **XFROM runtime (external only)**: `XFROM_RUNTIME=1` (module always external). Place `XFROMMAN.IRX` on `mc0:/SYS-CONF/` and test `XFROM_ENABLE=1/0`; repeat with only mc1 populated.
- [ ] **XFROM + HDD runtime**: `XFROM_RUNTIME=1 HDD_RUNTIME=1 HAS_EMBED_IRX=0`; verify both stacks load and config search order updates (`xfrom:/PS2BBL/CONFIG.INI`).
- [ ] **XFROM negative (no module present)**: `XFROM_RUNTIME=1`, omit `XFROMMAN.IRX`; ensure visible failure and continued boot.

## USB logging stacks (optional)
- [ ] **UDPTTY embedded**: `UDPTTY=1 HAS_EMBED_IRX=1 DEV9_NEED=1`. Confirm network bring-up and log reception.
- [ ] **UDPTTY external**: `UDPTTY=1 HAS_EMBED_IRX=0`; place `NETMAN.IRX`, `SMAP.IRX`, `PS2IP.IRX`, `UDPTTY.IRX` on `mc0:/SYS-CONF/`. Verify log output.
- [ ] **PPCTTY embedded**: `PPCTTY=1 HAS_EMBED_IRX=1`. Confirm UART log output.

## PSX-DESR (PSX=1) combinations
- [ ] **PSX build, embedded core only**: `PSX=1 HAS_EMBED_IRX=1`, no optional stacks. Confirm PS1 driver init, DVD player init, and DESR-specific IOP reset flow.
- [ ] **PSX + MX4SIO embedded**: `PSX=1 MX4SIO=1 HAS_EMBED_IRX=1`. Confirm `massX:/` availability and PSX pad/MC behavior.
- [ ] **PSX + MX4SIO runtime external**: `PSX=1 MX4SIO_RUNTIME=1`, stage `MX4SIO_BD.IRX` on `mc0:/SYS-CONF/`; test enable/disable keys.
- [ ] **PSX + MMCE embedded/runtime**: Repeat the two MMCE cases above with `PSX=1`.
- [ ] **PSX + HDD runtime external**: `PSX=1 HDD_RUNTIME=1`, external HDD IRX on `mc0:/SYS-CONF/`; verify enable key and pfs mount.

## CHAINLOAD mode
- [ ] **CHAINLOAD embedded IRX**: `CHAINLOAD=1 HAS_EMBED_IRX=1`, verify automatic boot of `CHAINLOAD_PATH` (`mc?:/BOOT/PAYLOAD.ELF`) and fallback to `mass:/RESCUE.ELF`.
- [ ] **CHAINLOAD external USB**: `CHAINLOAD=1 HAS_EMBED_IRX=0`, stage USB stack externally; confirm chainload still succeeds.

## External path variations (all stacks)
For each runtime/external test above, also verify:
- [ ] Modules only on `mc1:/SYS-CONF/` (mc0 empty).
- [ ] Mixed: core USB on mc0, optional stack IRX on mc1.
- [ ] Absence case: CONFIG requests enable but module file missing — verify clear on-screen error, boot continues.

## Reporting
For each checklist item, capture:
- Build flags used (exact make invocation)
- Console model/SCPH and region
- Storage/adapter details (HDD model, SD card type, MX4SIO adapter rev, USB stick model)
- Config source resolved and final CONFIG.INI contents for runtime enables
- Observed behavior (mount success, device listing, ELF launch, error texts)
- Pass/Fail result
