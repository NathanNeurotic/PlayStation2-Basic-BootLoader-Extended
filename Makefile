define HEADER
__________  _________________   ____________________.____
\______   \/   _____/\_____  \  \______   \______   \    |
 |     ___/\_____  \  /  ____/   |    |  _/|    |  _/    |
 |    |    /        \/       \   |    |   \|    |   \    |___
 |____|   /_______  /\_______ \  |______  /|______  /_______ \\
                  \/         \/         \/        \/        \/
		PlayStation2 Basic BootLoader - By El_isra
endef
export HEADER

# ---{ CANONICAL FEATURE TOGGLES }--- #
VARIANT ?= default

# All boolean feature toggles and their defaults live here to keep build logic and enumeration in sync.
FEATURE_BOOLEANS := HAS_EMBED_IRX DEBUG CHAINLOAD RUNTIME \
                    HDD HDD_CHAINLOAD HDD_RUNTIME \
                    MMCE MMCE_CHAINLOAD MMCE_RUNTIME \
                    MX4SIO MX4SIO_CHAINLOAD MX4SIO_RUNTIME \
                    USB USB_CHAINLOAD USB_RUNTIME \
                    XFROM XFROM_CHAINLOAD XFROM_RUNTIME \
                    PSX PROHBIT_DVD_0100 XCDVD_READKEY UDPTTY PPCTTY HOMEBREW_IRX FILEXIO_NEED DEV9_NEED \
                    KERNEL_NOPATCH NEWLIB_NANO DUMMY_TIMEZONE DUMMY_LIBC_INIT

# Default values (0/1) for each boolean toggle
DEFAULT_HAS_EMBED_IRX ?= 1 # whether to embed or not non vital IRX (which will be loaded from memcard files)
DEFAULT_DEBUG ?= 0
DEFAULT_CHAINLOAD ?= 0 # Only inits the system and boots CHAINLOAD_PATH from the memory card. If specified file doesn't exist, attempts to boot RESCUE.ELF from USB
DEFAULT_RUNTIME ?= 0
DEFAULT_PSX ?= 0 # PSX DESR support
DEFAULT_HDD ?= 0 # whether to add internal HDD support
DEFAULT_HDD_CHAINLOAD ?= 0
DEFAULT_HDD_RUNTIME ?= 0 # allow runtime HDD enablement via external IRX
DEFAULT_MMCE ?= 0
DEFAULT_MMCE_CHAINLOAD ?= 0
DEFAULT_MMCE_RUNTIME ?= 0
DEFAULT_MX4SIO ?= 0
DEFAULT_MX4SIO_CHAINLOAD ?= 0
DEFAULT_MX4SIO_RUNTIME ?= 0
DEFAULT_USB ?= 0
DEFAULT_USB_CHAINLOAD ?= 0
DEFAULT_USB_RUNTIME ?= 0
DEFAULT_XFROM ?= 0
DEFAULT_XFROM_CHAINLOAD ?= 0
DEFAULT_XFROM_RUNTIME ?= 0
DEFAULT_PROHBIT_DVD_0100 ?= 0 # prohibit the DVD Players v1.00 and v1.01 from being booted.
DEFAULT_XCDVD_READKEY ?= 0 # Enable the newer sceCdReadKey checks, which are only supported by a newer CDVDMAN module.
DEFAULT_UDPTTY ?= 0 # printf over UDP
DEFAULT_PPCTTY ?= 0 # printf over PowerPC UART
DEFAULT_HOMEBREW_IRX ?= 0 # if we need homebrew SIO2MAN, MCMAN, MCSERV & PADMAN embedded, else, builtin console drivers are used
DEFAULT_FILEXIO_NEED ?= 0 # if we need filexio and imanx loaded for other features (HDD, mx4sio, etc)
DEFAULT_DEV9_NEED ?= 0    # if we need DEV9 loaded for other features (HDD, UDPTTY, etc)
# Related to binary size reduction (it disables some features, please be sure you won't disable something you need)
DEFAULT_KERNEL_NOPATCH ?= 0
DEFAULT_NEWLIB_NANO ?= 1
DEFAULT_DUMMY_TIMEZONE ?= 1
DEFAULT_DUMMY_LIBC_INIT ?= 0

# Apply defaults using a single source of truth (treat empty env overrides as unset)
define init_feature_default
ifeq ($(strip $($(1))),)
  $(1) := $(DEFAULT_$(1))
endif
endef
$(foreach feature,$(FEATURE_BOOLEANS),$(eval $(call init_feature_default,$(feature))))

CHAINLOAD_PATH ?= mc?:/BOOT/PAYLOAD.ELF

# Multi-choice feature toggles
PRINTF_CHOICES := NONE PRINTF EE_SIO SCR
DEFAULT_PRINTF ?= NONE
PRINTF ?= $(DEFAULT_PRINTF)

CANONICAL_OPTIONS := $(FEATURE_BOOLEANS) PRINTF CHAINLOAD_PATH

COMMIT_HASH ?= $(shell git rev-parse --short HEAD)
SUPPRESS_FEATURE_INFO := $(if $(filter list-variants variants,$(MAKECMDGOALS)),1,)
feature_info = $(if $(SUPPRESS_FEATURE_INFO),,$(info $(1)))

# Normalize optional parameters
ifeq ($(strip $(PRINTF)),)
  PRINTF := NONE
endif

# ---{ OUTPUT LOCATIONS }--- #
VARIANTS_OUTDIR ?= build/variants
ifndef BINDIR
  OUTDIR ?= $(if $(filter default,$(VARIANT)),bin,$(VARIANTS_OUTDIR)/$(VARIANT))
  OUTDIR := $(patsubst %/,%,$(OUTDIR))
  BINDIR := $(OUTDIR)/
else
  OUTDIR := $(patsubst %/,%,$(BINDIR))
  BINDIR := $(OUTDIR)/
endif
EE_OBJS_DIR := $(OUTDIR)/obj/
EE_ASM_DIR := $(OUTDIR)/asm/
BATCH_MANIFEST = $(OUTDIR)/BATCH_MANIFEST.md
DEVICE_LIST := HDD MMCE MX4SIO USB XFROM
MODE_LABEL = $(strip $(if $(filter 1,$(2)),runtime$(if $(filter 1,$(1)),_chainload,),$(if $(filter 1,$(1)),chainload,normal)))
GLOBAL_MODE = $(call MODE_LABEL,$(CHAINLOAD),$(RUNTIME))
SELECTED_DEVICES = $(strip $(foreach dev,$(DEVICE_LIST),$(if $(or $(filter 1,$($(dev))),$(filter 1,$($(dev)_RUNTIME))),$(dev),)))
DEVICE_MODES = $(strip $(foreach dev,$(DEVICE_LIST),$(if $(or $(filter 1,$($(dev))),$(filter 1,$($(dev)_RUNTIME))),$(dev):$(call MODE_LABEL,$($(dev)_CHAINLOAD),$($(dev)_RUNTIME)),)))

# ---{ VARIANT ENUMERATION SUPPORT }--- #
# Invalid combinations expressed as pairs for validation and automatic matrix pruning
FEATURE_INVALID_PAIRS := \
  MMCE+MX4SIO \
  MMCE+MX4SIO_RUNTIME \
  MMCE_RUNTIME+MX4SIO \
  MMCE_RUNTIME+MX4SIO_RUNTIME \
  MMCE+MMCE_RUNTIME \
  MX4SIO+MX4SIO_RUNTIME \
  UDPTTY+PPCTTY \
  XFROM+XFROM_RUNTIME

# ---{ VERSIONING }--- #

VERSION = 1
SUBVERSION = 2
PATCHLEVEL = 0
STATUS = Beta

# ---{ EXECUTABLES }--- #

BASENAME ?= PS2BBL
EE_BIN = $(BINDIR)$(BASENAME).ELF
EE_BIN_STRIPPED = $(BINDIR)stripped_$(BASENAME).ELF
EE_BIN_PACKED = $(BINDIR)COMPRESSED_$(BASENAME).ELF
KELFTYPE ?= MC
EE_BIN_ENCRYPTED = $(BINDIR)$(BASENAME)_$(KELFTYPE).KELF
BUILD_CONFIG = $(OUTDIR)/BUILD_CONFIG.txt

# ---{ OBJECTS & STUFF }--- #

EE_OBJS_DIR = obj/
EE_SRC_DIR = src/
EE_ASM_DIR = asm/

EE_OBJS = main.o \
          globals.o \
          util.o common.o banner.o elf.o timer.o ps2.o ps1.o dvdplayer.o \
          modelname.o libcdvd_add.o OSDHistory.o OSDInit.o OSDConfig.o \
          $(EMBEDDED_STUFF) \
          $(IOP_OBJS)

EMBEDDED_STUFF = icon_sys_A.o icon_sys_J.o icon_sys_C.o

EE_CFLAGS = -Wall
EE_CFLAGS += -fno-common
EE_CFLAGS += -fdata-sections -ffunction-sections -DREPORT_FATAL_ERRORS
EE_LDFLAGS += -L$(PS2SDK)/ports/lib
EE_LDFLAGS += -Wl,--gc-sections -Wno-sign-compare
EE_LIBS += -ldebug -lmc -lpatches
EE_INCS += -Iinclude -I$(PS2SDK)/ports/include
EE_CFLAGS += -DVERSION=\"$(VERSION)\" -DSUBVERSION=\"$(SUBVERSION)\" -DPATCHLEVEL=\"$(PATCHLEVEL)\" -DSTATUS=\"$(STATUS)\"

# ---{ CONDITIONS }--- #

ifneq ($(VERBOSE), 1)
   .SILENT:
endif

# ---{ VALIDATION }--- #
$(foreach f,$(FEATURE_BOOLEANS),$(if $(filter 0 1,$(strip $($(f)))),,\
  $(error $(f) must be 0 or 1 (got '$(strip $($(f)))'))))

ifeq ($(filter $(PRINTF_CHOICES),$(PRINTF)),)
  $(error PRINTF must be one of: $(PRINTF_CHOICES))
endif

ifneq ($(and $(filter 1,$(MMCE)),$(filter 1,$(MX4SIO))),)
  $(error MMCE cannot coexist with MX4SIO)
endif
ifneq ($(and $(filter 1,$(MMCE)),$(filter 1,$(MX4SIO_RUNTIME))),)
  $(error MMCE cannot coexist with MX4SIO_RUNTIME)
endif
ifneq ($(and $(filter 1,$(MMCE_RUNTIME)),$(filter 1,$(MX4SIO))),)
  $(error MMCE_RUNTIME cannot coexist with MX4SIO)
endif
ifneq ($(and $(filter 1,$(MMCE_RUNTIME)),$(filter 1,$(MX4SIO_RUNTIME))),)
  $(error MMCE_RUNTIME cannot coexist with MX4SIO_RUNTIME)
endif
ifneq ($(and $(filter 1,$(MMCE)),$(filter 1,$(MMCE_RUNTIME))),)
  $(error MMCE cannot coexist with MMCE_RUNTIME)
endif
ifneq ($(and $(filter 1,$(MX4SIO)),$(filter 1,$(MX4SIO_RUNTIME))),)
  $(error MX4SIO cannot coexist with MX4SIO_RUNTIME)
endif
ifneq ($(and $(filter 1,$(UDPTTY)),$(filter 1,$(PPCTTY))),)
  $(error UDPTTY cannot coexist with PPCTTY)
endif
ifneq ($(and $(filter 1,$(XFROM)),$(filter 1,$(XFROM_RUNTIME))),)
  $(error XFROM cannot coexist with XFROM_RUNTIME)
endif

CHAINLOAD := $(if $(filter 1,$(CHAINLOAD) $(HDD_CHAINLOAD) $(MMCE_CHAINLOAD) $(MX4SIO_CHAINLOAD) $(USB_CHAINLOAD) $(XFROM_CHAINLOAD)),1,0)

ifeq ($(CHAINLOAD), 1)
  HAS_EMBED_IRX = 1
  EE_CFLAGS += -DCHAINLOAD -DCHAINLOAD_PATH=\"$(CHAINLOAD_PATH)\"
endif

ifeq ($(RUNTIME), 1)
  EE_CFLAGS += -DRUNTIME
endif

ifeq ($(MX4SIO), 1)
  HOMEBREW_IRX = 1
  FILEXIO_NEED = 1
  EE_OBJS += mx4sio_bd_irx.o
  EE_CFLAGS += -DMX4SIO
  ifeq ($(USE_ROM_SIO2MAN), 1)
    $(error MX4SIO needs Homebrew SIO2MAN to work)
  endif
endif

ifeq ($(MMCE), 1)
  HOMEBREW_IRX = 1
  FILEXIO_NEED = 1
  EE_OBJS += mmceman_irx.o
  EE_CFLAGS += -DMMCE
  ifeq ($(USE_ROM_SIO2MAN), 1)
    $(error MMCE needs Homebrew SIO2MAN to work)
  endif
endif

ifeq ($(MX4SIO_RUNTIME), 1)
  HOMEBREW_IRX = 1
  FILEXIO_NEED = 1
  EE_OBJS += mx4sio_bd_irx.o
  EE_CFLAGS += -DMX4SIO_RUNTIME
  ifeq ($(USE_ROM_SIO2MAN), 1)
    $(error MX4SIO runtime needs Homebrew SIO2MAN to work)
  endif
endif

ifeq ($(MMCE_RUNTIME), 1)
  HOMEBREW_IRX = 1
  FILEXIO_NEED = 1
  EE_OBJS += mmceman_irx.o
  EE_CFLAGS += -DMMCE_RUNTIME
  ifeq ($(USE_ROM_SIO2MAN), 1)
    $(error MMCE runtime needs Homebrew SIO2MAN to work)
  endif
endif

ifeq ($(XFROM), 1)
  FILEXIO_NEED = 1
  EE_OBJS += xfromman_irx.o extflash_irx.o
  EE_CFLAGS += -DXFROM
endif

ifeq ($(XFROM_RUNTIME), 1)
  FILEXIO_NEED = 1
  EE_OBJS += xfromman_irx.o extflash_irx.o
  EE_CFLAGS += -DXFROM_RUNTIME
endif

ifeq ($(HOMEBREW_IRX), 1)
   $(call feature_info,--- enforcing usage of homebrew IRX modules)
   USE_ROM_PADMAN = 0
   USE_ROM_MCMAN = 0
   USE_ROM_SIO2MAN = 0
else
   $(call feature_info,--- using BOOT-ROM drivers)
   USE_ROM_PADMAN = 1
   USE_ROM_MCMAN = 1
   USE_ROM_SIO2MAN = 1
endif

ifeq ($(PSX), 1)
   $(call feature_info,--- building with PSX-DESR support)
  BASENAME = PSXBBL
  EE_CFLAGS += -DPSX=1
  EE_OBJS += scmd_add.o ioprp.o
  EE_LIBS += -lxcdvd -liopreboot
else
  EE_LIBS += -lcdvd
endif

ifeq ($(DEBUG), 1)
   $(call feature_info,--- debugging enabled)
  EE_CFLAGS += -DDEBUG -O0 -g
  EE_LIBS += -lelf-loader
else
  EE_CFLAGS += -Os
  EE_LDFLAGS += -s
  EE_LIBS += -lelf-loader-nocolour
endif

ifeq ($(USE_ROM_PADMAN), 1)
  EE_CFLAGS += -DUSE_ROM_PADMAN
  EE_LIBS += -lpad
  EE_OBJS += pad.o
else
  EE_OBJS += pad.o padman_irx.o
  EE_LIBS += -lpadx
endif

ifeq ($(USE_ROM_MCMAN), 1)
  EE_CFLAGS += -DUSE_ROM_MCMAN
else
  EE_OBJS += mcman_irx.o mcserv_irx.o
endif

ifeq ($(USE_ROM_SIO2MAN), 1)
  EE_CFLAGS += -DUSE_ROM_SIO2MAN
else
  EE_OBJS += sio2man_irx.o
endif

ifeq ($(HAS_EMBED_IRX), 1)
  $(call feature_info,--- USB drivers will be embedded)
  EE_OBJS += usbd_irx.o bdm_irx.o bdmfs_fatfs_irx.o usbmass_bd_irx.o
  EE_CFLAGS += -DHAS_EMBEDDED_IRX
else
  $(call feature_info,--- USB drivers will be external)
endif

ifeq ($(HDD), 1)
  $(call feature_info,--- compiling with HDD support)
  EE_LIBS += -lpoweroff
  EE_OBJS += ps2fs_irx.o ps2hdd_irx.o ps2atad_irx.o poweroff_irx.o
  EE_CFLAGS += -DHDD
  FILEXIO_NEED = 1
  DEV9_NEED = 1
  KELFTYPE = HDD
endif

ifeq ($(HDD_RUNTIME), 1)
  $(call feature_info,--- runtime HDD enablement support enabled)
  EE_CFLAGS += -DHDD_RUNTIME
  FILEXIO_NEED = 1
  DEV9_NEED = 1
  EE_LIBS += -lpoweroff
endif

ifeq ($(UDPTTY), 1)
  $(call feature_info,--- UDPTTY enabled)
  EE_CFLAGS += -DUDPTTY
  EE_OBJS += udptty_irx.o ps2ip_irx.o netman_irx.o smap_irx.o
  DEV9_NEED = 1
  ifneq ($(PRINTF), EE_SIO) # only enable common printf if EE_SIO is disabled. this allows separating EE and IOP printf
    PRINTF = PRINTF
  endif
else ifeq ($(PPCTTY), 1)
  $(call feature_info,--- PPCTTY enabled)
  EE_CFLAGS += -DPPCTTY
  EE_OBJS += ppctty_irx.o
  ifneq ($(PRINTF), EE_SIO) # only enable common printf if EE_SIO is disabled. this allows separating EE and IOP printf
    PRINTF = PRINTF
  endif
endif

ifeq ($(FILEXIO_NEED), 1)
  $(call feature_info,--- FILEXIO will be included)
  EE_CFLAGS += -DFILEXIO
  EE_LIBS += -lfileXio
  EE_OBJS += filexio_irx.o iomanx_irx.o
endif

ifeq ($(DUMMY_TIMEZONE), 1)
  EE_CFLAGS += -DDUMMY_TIMEZONE
endif

ifeq ($(NEWLIB_NANO), 1)
  EE_CFLAGS += -DNEWLIB_NANO
endif

ifeq ($(DEV9_NEED), 1)
  EE_CFLAGS += -DDEV9
  EE_OBJS += ps2dev9_irx.o
endif

ifdef COMMIT_HASH
  EE_CFLAGS += -DCOMMIT_HASH=\"$(COMMIT_HASH)\"
else
  EE_CFLAGS += -DCOMMIT_HASH=\"$(shell git rev-parse --short HEAD)\"
endif

ifeq ($(DUMMY_LIBC_INIT), 1)
  EE_CFLAGS += -DDUMMY_LIBC_INIT
endif

ifeq ($(KERNEL_NOPATCH), 1)
  EE_CFLAGS += -DKERNEL_NOPATCH
endif

ifeq ($(PRINTF), NONE)
else ifeq ($(PRINTF), SCR)
  $(call feature_info,--- SCR Printf enabled)
  EE_CFLAGS += -DSCR_PRINT
else ifeq ($(PRINTF), EE_SIO)
  $(call feature_info,--- EESIO Printf enabled)
  EE_CFLAGS += -DEE_SIO_DEBUG
  EE_LIBS += -lsiocookie
  EE_OBJS += sioprintf.o
else ifeq ($(PRINTF), PRINTF)
  $(call feature_info,--- Common Printf enabled)
  EE_CFLAGS += -DCOMMON_PRINTF
else ifneq ($(PRINTF),)
  $(warning UNKNOWN PRINTF REQUESTED: '$(PRINTF)')
endif

ifeq ($(XCDVD_READKEY),1)
  EE_CFLAGS += -DXCDVD_READKEY=1
endif

ifeq ($(PROHBIT_DVD_0100),1)
  EE_CFLAGS += -DPROHBIT_DVD_0100=1
endif

# ---{ RECIPES }--- #
.PHONY: greeting debug all clean kelf packed release variants list-variants batch-notes
.ONESHELL: list-variants variants

all: $(EE_BIN) $(BUILD_CONFIG)
ifeq (DEBUG, 1)
	$(MAKE) greeting
endif
rebuild: clean packed

packed: $(EE_BIN_PACKED) $(BUILD_CONFIG)

greeting:
	@echo built PS2BBL PSX=$(PSX), LOCAL_IRX=$(HAS_EMBED_IRX), DEBUG=$(DEBUG)
	@echo PROHBIT_DVD_0100=$(PROHBIT_DVD_0100), XCDVD_READKEY=$(XCDVD_READKEY)
	@echo KERNEL_NOPATCH=$(KERNEL_NOPATCH), NEWLIB_NANO=$(NEWLIB_NANO)
	@echo binaries dispatched to $(BINDIR)
	@echo printf=$(PRINTF)
	@echo $(EE_OBJS)

release: clean $(EE_BIN_PACKED) $(BUILD_CONFIG)
	@rm -f $(EE_BIN_STRIPPED)
	@$(MAKE) --no-print-directory batch-notes
	@echo "$$HEADER"

clean:
	@rm -rf $(EE_BIN) $(EE_BIN_STRIPPED) $(EE_BIN_ENCRYPTED) $(EE_BIN_PACKED) $(BUILD_CONFIG) $(BATCH_MANIFEST)
	@rm -rf $(EE_OBJS_DIR) $(EE_ASM_DIR)

$(EE_BIN_STRIPPED): $(EE_BIN)
	@echo " -- Stripping"
	$(EE_STRIP) -o $@ $<

$(EE_BIN_PACKED): $(EE_BIN_STRIPPED)
	@echo " -- Compressing"
ifneq ($(DEBUG),1)
	ps2-packer $< $@
else
	ps2-packer -v $< $@
endif

$(EE_BIN_ENCRYPTED): $(EE_BIN_PACKED)
	@echo " -- Encrypting ($(KELFTYPE))"
ifeq ($(KELFTYPE), MC)
	thirdparty/kelftool encrypt dnasload $< $@
else ifeq ($(KELFTYPE), HDD)
	thirdparty/kelftool encrypt fhdb $< $@
else
	$(error UNKNOWN KELF TYPE: '$(KELFTYPE)')
endif

$(BUILD_CONFIG): $(EE_BIN)
	@mkdir -p $(OUTDIR)
	@echo "VARIANT=$(VARIANT)" > $@
	@echo "COMMIT_HASH=$(COMMIT_HASH)" >> $@
	@echo "UTC_TIME=$(shell date -u '+%Y-%m-%dT%H:%M:%SZ')" >> $@
	@echo "OUTDIR=$(OUTDIR)" >> $@
	@echo "BASENAME=$(BASENAME)" >> $@
	@echo "DEVICES=$(if $(SELECTED_DEVICES),$(SELECTED_DEVICES),NONE)" >> $@
	@echo "GLOBAL_MODE=$(GLOBAL_MODE)" >> $@
	@echo "DEVICE_MODES=$(if $(DEVICE_MODES),$(DEVICE_MODES),NONE)" >> $@
	@echo "OPTIONS" >> $@
	@for opt in $(CANONICAL_OPTIONS); do echo "  $$opt=$($(opt))"; done >> $@
	@echo "MAKEOVERRIDES=$(MAKEOVERRIDES)" >> $@
	@echo "FLAGS=$(strip $(MAKEOVERRIDES))" >> $@
	@echo "CFLAGS=$(EE_CFLAGS)" >> $@
	@echo "DEFINES=$(filter -D%,$(EE_CFLAGS))" >> $@
# move OBJ to folder and search source on src/, borrowed from OPL makefile

EE_OBJS := $(EE_OBJS:%=$(EE_OBJS_DIR)%) # remap all EE_OBJ to obj subdir

$(EE_OBJS_DIR):
	@mkdir -p $@

$(EE_ASM_DIR):
	@mkdir -p $@

$(BINDIR):
	@mkdir -p $@

vpath %.c $(EE_ASM_DIR)
$(EE_OBJS_DIR)%.o: $(EE_SRC_DIR)%.c | $(EE_OBJS_DIR)
ifneq ($(VERBOSE),1)
	@echo "  - $@"
endif
	$(EE_CC) $(EE_CFLAGS) $(EE_INCS) -c $< -o $@

$(EE_OBJS_DIR)%.o: $(EE_ASM_DIR)%.c | $(EE_OBJS_DIR)
ifneq ($(VERBOSE),1)
	@echo "  - $@"
endif
	$(EE_CC) $(EE_CFLAGS) $(EE_INCS) -c $< -o $@

$(EE_OBJS_DIR)%.o: $(EE_ASM_DIR)%.s | $(EE_OBJS_DIR)
ifneq ($(VERBOSE),1)
	@echo "  - $@"
endif
	$(EE_AS) $(EE_ASFLAGS) $< -o $@

batch-notes: $(EE_BIN_PACKED) $(BUILD_CONFIG)
	@python3 scripts/generate_batch_manifest.py --outdir "$(OUTDIR)" --dest "$(BATCH_MANIFEST)" --name "$(VARIANT)"

list-variants:
	@python3 scripts/list_variants.py

variants:
	@matrix="$$( $(MAKE) --no-print-directory list-variants )"; \
	python3 - <<-'PY'
		import json, os, subprocess, shlex
		matrix = json.loads("""$${matrix}""")
		include = matrix.get("include", [])
		for entry in include:
			name = entry["name"]
			flags = entry.get("flags", "").strip()
			outdir = os.path.join("$(VARIANTS_OUTDIR)", name)
			base = ["$(MAKE)", f"VARIANT={name}", f"OUTDIR={outdir}"] + (shlex.split(flags) if flags else [])
			subprocess.run(base + ["clean"], check=True)
			subprocess.run(base + ["all"], check=True)
			cfg = os.path.join(outdir, "BUILD_CONFIG.txt")
			subprocess.run(["/bin/sh", "-c", f"test -f {cfg}"], check=True)
			subprocess.run(base + ["batch-notes"], check=True)
	PY
#
analize:
	$(MAKE) rebuild DEBUG=1
	python3 thirdparty/elf-size-analize.py $(EE_BIN) -R -t mips64r5900el-ps2-elf-

celan: clean # a repetitive typo when quicktyping
kelf: $(EE_BIN_ENCRYPTED) # alias of KELF creation


banner:
	@echo "$$HEADER"

# Include makefiles
ifneq ($(filter list-variants,$(MAKECMDGOALS)),)
  # Skip heavy includes when only enumerating variants
else
  include $(PS2SDK)/samples/Makefile.pref
  include $(PS2SDK)/samples/Makefile.eeglobal
  include embed.make
endif
