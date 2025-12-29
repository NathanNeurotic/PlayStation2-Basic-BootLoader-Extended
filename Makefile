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

# ---{ CANONICAL BUILD OPTIONS }--- #
VARIANT ?= default

# Device and platform feature toggles
HAS_EMBED_IRX ?= 1 # whether to embed or not non vital IRX (wich will be loaded from memcard files)
DEBUG ?= 0
CHAINLOAD ?= 0 # Only inits the system and boots CHAINLOAD_PATH from the memory card. If specified file doesn't exist, attempts to boot RESCUE.ELF from USB
CHAINLOAD_PATH ?= mc?:/BOOT/PAYLOAD.ELF
PSX ?= 0 # PSX DESR support
HDD ?= 0 #wether to add internal HDD support
MMCE ?= 0
MX4SIO ?= 0
XFROM ?= 0
PROHBIT_DVD_0100 ?= 0 # prohibit the DVD Players v1.00 and v1.01 from being booted.
XCDVD_READKEY ?= 0 # Enable the newer sceCdReadKey checks, which are only supported by a newer CDVDMAN module.
UDPTTY ?= 0 # printf over UDP
PPCTTY ?= 0 # printf over PowerPC UART
PRINTF ?= NONE
HDD_RUNTIME ?= 0 # allow runtime HDD enablement via external IRX
MX4SIO_RUNTIME ?= 0
MMCE_RUNTIME ?= 0
XFROM_RUNTIME ?= 0

HOMEBREW_IRX ?= 0 # if we need homebrew SIO2MAN, MCMAN, MCSERV & PADMAN embedded, else, builtin console drivers are used
FILEXIO_NEED ?= 0 # if we need filexio and imanx loaded for other features (HDD, mx4sio, etc)
DEV9_NEED ?= 0    # if we need DEV9 loaded for other features (HDD, UDPTTY, etc)

# Related to binary size reduction (it disables some features, please be sure you won't disable something you need)
KERNEL_NOPATCH ?= 1
NEWLIB_NANO ?= 1
DUMMY_TIMEZONE ?= 1
DUMMY_LIBC_INIT ?= 0

CANONICAL_OPTIONS := HAS_EMBED_IRX DEBUG CHAINLOAD CHAINLOAD_PATH PSX HDD HDD_RUNTIME MMCE MMCE_RUNTIME MX4SIO MX4SIO_RUNTIME \
                     XFROM XFROM_RUNTIME PROHBIT_DVD_0100 XCDVD_READKEY UDPTTY PPCTTY PRINTF HOMEBREW_IRX FILEXIO_NEED DEV9_NEED \
                     KERNEL_NOPATCH NEWLIB_NANO DUMMY_TIMEZONE DUMMY_LIBC_INIT

COMMIT_HASH ?= $(shell git rev-parse --short HEAD)
SUPPRESS_FEATURE_INFO := $(if $(filter list-variants variants,$(MAKECMDGOALS)),1,)
feature_info = $(if $(SUPPRESS_FEATURE_INFO),,$(info $(1)))

# ---{ OUTPUT LOCATIONS }--- #
ifndef BINDIR
  OUTDIR ?= $(if $(filter default,$(VARIANT)),bin,build/out/$(VARIANT))
  OUTDIR := $(patsubst %/,%,$(OUTDIR))
  BINDIR := $(OUTDIR)/
else
  OUTDIR := $(patsubst %/,%,$(BINDIR))
  BINDIR := $(OUTDIR)/
endif
EE_OBJS_DIR := $(OUTDIR)/obj/
EE_ASM_DIR := $(OUTDIR)/asm/
VARIANTS_OUTDIR ?= build/variants

# ---{ VARIANT DEFINITIONS }--- #
VARIANT_DEFINITIONS := \
  default: \
  chainload:CHAINLOAD=1 \
  hdd:HDD=1 \
  hdd_chainload:HDD=1,CHAINLOAD=1 \
  hdd_runtime:HDD_RUNTIME=1 \
  hdd_runtime_chainload:HDD_RUNTIME=1,CHAINLOAD=1 \
  mmce:MMCE=1 \
  mmce_chainload:MMCE=1,CHAINLOAD=1 \
  mmce_runtime:MMCE_RUNTIME=1 \
  mmce_runtime_chainload:MMCE_RUNTIME=1,CHAINLOAD=1 \
  mx4sio:MX4SIO=1 \
  mx4sio_chainload:MX4SIO=1,CHAINLOAD=1 \
  mx4sio_runtime:MX4SIO_RUNTIME=1 \
  mx4sio_runtime_chainload:MX4SIO_RUNTIME=1,CHAINLOAD=1 \
  xfrom:XFROM=1 \
  xfrom_chainload:XFROM=1,CHAINLOAD=1 \
  xfrom_runtime:XFROM_RUNTIME=1 \
  xfrom_runtime_chainload:XFROM_RUNTIME=1,CHAINLOAD=1 \
  psx:PSX=1 \
  psx_chainload:PSX=1,CHAINLOAD=1 \
  psx_hdd:PSX=1,HDD=1 \
  psx_hdd_chainload:PSX=1,HDD=1,CHAINLOAD=1 \
  psx_hdd_runtime:PSX=1,HDD_RUNTIME=1 \
  psx_hdd_runtime_chainload:PSX=1,HDD_RUNTIME=1,CHAINLOAD=1 \
  psx_mmce:PSX=1,MMCE=1 \
  psx_mmce_chainload:PSX=1,MMCE=1,CHAINLOAD=1 \
  psx_mmce_runtime:PSX=1,MMCE_RUNTIME=1 \
  psx_mmce_runtime_chainload:PSX=1,MMCE_RUNTIME=1,CHAINLOAD=1 \
  psx_mx4sio:PSX=1,MX4SIO=1 \
  psx_mx4sio_chainload:PSX=1,MX4SIO=1,CHAINLOAD=1 \
  psx_mx4sio_runtime:PSX=1,MX4SIO_RUNTIME=1 \
  psx_mx4sio_runtime_chainload:PSX=1,MX4SIO_RUNTIME=1,CHAINLOAD=1 \
  psx_xfrom:PSX=1,XFROM=1 \
  psx_xfrom_chainload:PSX=1,XFROM=1,CHAINLOAD=1 \
  psx_xfrom_runtime:PSX=1,XFROM_RUNTIME=1 \
  psx_xfrom_runtime_chainload:PSX=1,XFROM_RUNTIME=1,CHAINLOAD=1 \
  udptty:UDPTTY=1 \
  ppctty:PPCTTY=1

variant_name = $(word 1,$(subst :, ,$1))
variant_flags = $(wordlist 2,200,$(subst :, ,$1))
variant_json = {"name":"$(call variant_name,$1)","flags":"$(strip $(call variant_flags,$1))"}

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
ifeq ($(filter 0 1,$(HAS_EMBED_IRX)),)
  $(error HAS_EMBED_IRX must be 0 or 1)
endif
ifeq ($(filter 0 1,$(DEBUG)),)
  $(error DEBUG must be 0 or 1)
endif
ifeq ($(filter 0 1,$(CHAINLOAD)),)
  $(error CHAINLOAD must be 0 or 1)
endif
ifeq ($(filter 0 1,$(PSX)),)
  $(error PSX must be 0 or 1)
endif
ifeq ($(filter 0 1,$(HDD) $(HDD_RUNTIME) $(MMCE) $(MMCE_RUNTIME) $(MX4SIO) $(MX4SIO_RUNTIME) $(XFROM) $(XFROM_RUNTIME) $(UDPTTY) $(PPCTTY)),)
  $(error Storage/debug feature toggles must be 0 or 1)
endif
ifeq ($(filter NONE PRINTF EE_SIO SCR,$(PRINTF)),)
  $(error PRINTF must be one of: NONE, PRINTF, EE_SIO, SCR)
endif

ifeq ($(CHAINLOAD), 1)
  HAS_EMBED_IRX = 1
  EE_CFLAGS += -DCHAINLOAD -DCHAINLOAD_PATH=\"$(CHAINLOAD_PATH)\"
  undefine EMBEDDED_STUFF
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
  ifeq ($(MX4SIO), 1)
    $(error MX4SIO cant coexist with MMCE)
  endif
endif

ifeq ($(MX4SIO_RUNTIME), 1)
  HOMEBREW_IRX = 1
  FILEXIO_NEED = 1
  EE_CFLAGS += -DMX4SIO_RUNTIME
  ifeq ($(MX4SIO), 1)
    $(error MX4SIO_RUNTIME cant coexist with MX4SIO)
  endif
  ifeq ($(MMCE), 1)
    $(error MX4SIO_RUNTIME cant coexist with MMCE)
  endif
  ifeq ($(MMCE_RUNTIME), 1)
    $(error MX4SIO_RUNTIME cant coexist with MMCE_RUNTIME)
  endif
  ifeq ($(USE_ROM_SIO2MAN), 1)
    $(error MX4SIO runtime needs Homebrew SIO2MAN to work)
  endif
endif

ifeq ($(MMCE_RUNTIME), 1)
  HOMEBREW_IRX = 1
  FILEXIO_NEED = 1
  EE_CFLAGS += -DMMCE_RUNTIME
  ifeq ($(MMCE), 1)
    $(error MMCE_RUNTIME cant coexist with MMCE)
  endif
  ifeq ($(MX4SIO), 1)
    $(error MX4SIO cant coexist with MMCE_RUNTIME)
  endif
  ifeq ($(MX4SIO_RUNTIME), 1)
    $(error MX4SIO_RUNTIME cant coexist with MMCE_RUNTIME)
  endif
  ifeq ($(USE_ROM_SIO2MAN), 1)
    $(error MMCE runtime needs Homebrew SIO2MAN to work)
  endif
endif

ifeq ($(XFROM), 1)
  FILEXIO_NEED = 1
  EE_CFLAGS += -DXFROM
endif

ifeq ($(XFROM_RUNTIME), 1)
  FILEXIO_NEED = 1
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
.PHONY: greeting debug all clean kelf packed release variants list-variants

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
	@echo "$$HEADER"

clean:
	@rm -rf $(EE_BIN) $(EE_BIN_STRIPPED) $(EE_BIN_ENCRYPTED) $(EE_BIN_PACKED) $(BUILD_CONFIG)
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
	@echo "OPTIONS" >> $@
	@for opt in $(CANONICAL_OPTIONS); do echo "  $$opt=$($(opt))"; done >> $@
	@echo "MAKEOVERRIDES=$(MAKEOVERRIDES)" >> $@
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

list-variants:
	@sep=""
	@printf '{"include":['
	@for entry in $(VARIANT_DEFINITIONS); do \
		name=$${entry%%:*}; \
		flags=$${entry#*:}; \
		if [ "$$entry" = "$$name" ]; then flags=""; fi; \
		flags=$$(echo "$$flags" | sed 's/,/ /g'); \
		printf '%s{"name":"%s","flags":"%s"}' "$$sep" "$$name" "$$flags"; \
		sep=","; \
	done; \
	printf ']}\n'

variants:
	@set -e; \
	for entry in $(VARIANT_DEFINITIONS); do \
		name=$${entry%%:*}; \
		flags=$${entry#*:}; \
		if [ "$$entry" = "$$name" ]; then flags=""; fi; \
		flags=$$(echo "$$flags" | sed 's/,/ /g'); \
		odir="$(VARIANTS_OUTDIR)/$$name"; \
		$(MAKE) VARIANT=$$name OUTDIR=$$odir $$flags clean >/dev/null; \
		$(MAKE) VARIANT=$$name OUTDIR=$$odir $$flags all; \
	done
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
