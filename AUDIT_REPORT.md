# PS2BBL Extended - Technical Audit Report

## 1. Path Resolution Logic
*   **Status:** **FAIL**
*   **Finding:** The config search order implemented in `main.c` does not match the documentation in `README.md`.
    *   **Documentation Claim:** `CONFIG.INI` (CWD) -> `mc1` -> `mc0` -> `mc?` (PSX) -> `mmce` -> `xfrom` -> `hdd0` -> `massX` -> `mass`.
    *   **Implementation Reality:** `CONFIG.INI` (CWD) -> `mc?` (PSX) -> `mmce` -> `xfrom` -> `hdd0` -> `massX` -> `mass` -> `mc1` -> `mc0`.
    *   **Root Cause:** The search loop in `main.c` iterates backwards (`x = SOURCE_CWD; x >= SOURCE_MC0; x--`). Since `SOURCE_MASS` (index 2) is checked before `SOURCE_MC1` (index 1), `mass` takes precedence over `mc`.
    *   **Risk:** Users expecting Memory Card config to override USB Mass Storage config will experience the opposite behavior. This is a significant usability bug and regression from standard behavior.
*   **Ambiguity:** `CheckPath` for `mc?:` resolves contextually based on where the config was found (`config_source`), which is good design but undocumented.

## 2. Regression Safety
*   **Status:** **FAIL**
*   **Finding:** Behavioral divergence from upstream PS2BBL.
    *   Upstream typically prioritizes Memory Card over Mass Storage.
    *   This fork prioritizes Mass Storage over Memory Card (due to the loop order issue mentioned above).
    *   This change is likely unintentional given the README claims MC is higher priority.

## 3. Boot Stability & Edge Cases
*   **Status:** **PASS** (with minor caveats)
*   **Missing Config:** Handled correctly. Defaults are loaded.
*   **Corrupt Config:** Config parser handles EOF and partial reads gracefully.
*   **Multiple Configs:** The "first found" logic is deterministic (albeit in the wrong order currently).
*   **Unsupported Devices:** `IsConfigPathEnabled` correctly filters out disabled devices.
*   **Caveat:** `LookForBDMDevice` (MX4SIO probe) is called inside the search loop. If `MX4SIO` is enabled (embedded), it probes 5 USB devices. This adds boot delay before checking Memory Cards (due to the loop order).

## 4. Code Quality & Maintainability
*   **Status:** **PASS**
*   **Redundant Logic:** `EnableConfigPath` is called inside `StartMX4SIO` (and others) even when called from the runtime enablement block. Since the config search loop has already finished by then, this `EnableConfigPath` call has no effect on config loading (though it sets the state for later checks).
*   **Magic Constants:** `SOURCE_COUNT`, `KEYS_ID` usage is consistent.
*   **Readability:** Code is generally readable. The mismatch between `DEFAULT_CONFIG_PATHS` array order and `CONFIG_SOURCES_ID` enum order (if any) requires careful cross-referencing.

## 5. Security & Safety
*   **Status:** **PASS**
*   **Buffer Risks:** `sprintf` and `snprintf` are used with appropriate buffer sizes. Config file reading uses dynamic allocation based on file size + null terminator.
*   **Path Traversal:** `LocateExternalIRXPath` uses fixed templates. Input `filename` from config is inserted. While `../` traversal is theoretically possible if the filesystem supports it, the impact is limited to loading IRX modules from unintended locations on the same device.
*   **Input Validation:** Config keys are string-compared safely. Values are copied with `strdup` or checked for length before `strcpy` (in `StoreKeypathFallback`).

## 6. Performance Impact
*   **Status:** **PASS**
*   **I/O:** `LookForBDMDevice` performs I/O (directory open/ioctl) on `mass0`..`mass4`. This only happens if MX4SIO is enabled.
*   **Optimization:** The search loop breaks immediately upon finding a config.

## 7. Documentation Accuracy
*   **Status:** **FAIL**
*   **Findings:**
    *   **Search Order:** Completely contradictory to code implementation regarding MC vs Mass priority.
    *   **Runtime Enablement:** The README implies that `MX4SIO_ENABLE=1` "enables `massX:/PS2BBL/CONFIG.INI`". In reality, since the config file is already loaded by the time this key is processed, enabling `massX` does not allow it to be used as a *config source* for the current boot session. It only makes the path available for other uses (like `CheckPath` resolution for ELFs).

## Verdict
**FAIL** - Determining path resolution order is critical for a bootloader. The implementation actively contradicts the documentation and introduces a regression where Mass Storage overrides Memory Card configuration.

## Recommendations
1.  **Fix Search Order:** Reverse the config search loop or reorder the `DEFAULT_CONFIG_PATHS`/`CONFIG_SOURCES_ID` to strictly follow the documented precedence (MC > Mass).
2.  **Update Documentation:** Clarify that runtime `*_ENABLE` keys do not allow loading the *main* config from those devices (chicken-and-egg problem), or implement a "chain load" mechanism if that is the intent.
3.  **Optimize Probing:** Ensure device probing (like `LookForBDMDevice`) only happens if the device is actually enabled and the path is being checked.
4.  **Clarify `massX` Logic:** Document that `massX` maps to the first found MX4SIO device (usually `mass0` if only one is present).
