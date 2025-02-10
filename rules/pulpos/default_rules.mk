TARGET_BUILD_DIR = $(CURDIR)/build$(build_dir_ext)

ifdef QUIET
V = @
endif

ifdef VERBOSE
override runner_args += --config-opt=**/runner/verbose=true
endif

platform ?= rtl
VSIM ?= vsim


ifdef PULP_RUNTIME_GCC_TOOLCHAIN
PULP_CC := $(PULP_RUNTIME_GCC_TOOLCHAIN)/bin/$(PULP_CC)
PULP_LD := $(PULP_RUNTIME_GCC_TOOLCHAIN)/bin/$(PULP_LD)
PULP_OBJDUMP := $(PULP_RUNTIME_GCC_TOOLCHAIN)/bin/$(PULP_OBJDUMP)
else
ifdef PULP_RISCV_GCC_TOOLCHAIN
PULP_CC := $(PULP_RISCV_GCC_TOOLCHAIN)/bin/$(PULP_CC)
PULP_LD := $(PULP_RISCV_GCC_TOOLCHAIN)/bin/$(PULP_LD)
PULP_OBJDUMP := $(PULP_RISCV_GCC_TOOLCHAIN)/bin/$(PULP_OBJDUMP)
else
$(warning "Warning: Neither PULP_RUNTIME_GCC_TOOLCHAIN nor PULP_RISCV_GCC_TOOLCHAIN is set.\
Using defaults.")
endif
endif


ifdef gui
override runner_args += --config-opt=**/vsim/gui=true
endif

ifdef io
ifeq '$(io)' 'uart'
CONFIG_IO_UART=1
endif
ifeq '$(io)' 'host_uart'
CONFIG_IO_UART=2
PULP_APP_CFLAGS += -I$(PULPRT_HOME)/drivers/include
endif
endif

VPATH = $(PULPRT_HOME)

include $(PULPRT_HOME)/rules/pulpos/src.mk

PULP_CFLAGS += $(PULPRT_CONFIG_CFLAGS)
PULP_CFLAGS += -fno-jump-tables -fno-tree-loop-distribute-patterns

ifeq '$(CONFIG_LIBC_MINIMAL)' '1'
PULP_APP_CFLAGS += -I$(PULPRT_HOME)/lib/libc/minimal/include
endif
PULP_APP_CFLAGS += -I$(PULPRT_HOME)/include -I$(PULPRT_HOME)/kernel

PULP_APP_CFLAGS += $(foreach inc,$(PULPOS_MODULES),-I$(inc)/include)

ifdef PULPRUN_PLATFORM
platform=$(PULPRUN_PLATFORM)
endif


override disopt ?= -d

ifeq '$(platform)' 'gvsoc'
PULP_CFLAGS += -D__PLATFORM__=ARCHI_PLATFORM_GVSOC
endif
ifeq '$(platform)' 'board'
PULP_CFLAGS += -D__PLATFORM__=ARCHI_PLATFORM_BOARD
endif
ifeq '$(platform)' 'rtl'
PULP_CFLAGS += -D__PLATFORM__=ARCHI_PLATFORM_RTL
endif
ifeq '$(platform)' 'fpga'
PULP_CFLAGS += -D__PLATFORM__=ARCHI_PLATFORM_FPGA
endif

ifdef CONFIG_NB_PE
PULP_CFLAGS += -DARCHI_CLUSTER_NB_PE=$(CONFIG_NB_PE)
endif

ifdef CONFIG_NO_FC
PULP_CFLAGS += -DARCHI_NO_FC=1
endif

ifdef CONFIG_CL_BOOT
PULP_CFLAGS += -DARCHI_CL_BOOT=1
endif

ifdef CONFIG_IO_UART
PULP_CFLAGS += -DCONFIG_IO_UART=$(CONFIG_IO_UART)
endif

ifdef CONFIG_IO_UART_BAUDRATE
PULP_CFLAGS += -DCONFIG_IO_UART_BAUDRATE=$(CONFIG_IO_UART_BAUDRATE)
endif

ifdef CONFIG_IO_UART_ITF
PULP_CFLAGS += -DCONFIG_IO_UART_ITF=$(CONFIG_IO_UART_ITF)
endif

ifdef CONFIG_RISCV_GENERIC
PULP_CFLAGS += -D__RISCV_GENERIC__=1
endif

ifdef CONFIG_USE_ASM_OPTIM
PULP_CFLAGS  += -D__USE_ASM_OPTIM__=1
endif

ifdef CONFIG_TRACE_LEVEL
PULP_CFLAGS += -D__TRACE_LEVEL__=$(CONFIG_TRACE_LEVEL)
endif

ifdef CONFIG_TRACE_ALL
PULP_CFLAGS += -D__TRACE_ALL__=1
endif

ifdef CONFIG_TRACE_SPIM
PULP_CFLAGS += -D__TRACE_SPIM__=1
endif

ifdef CONFIG_TRACE_FREQ
PULP_CFLAGS += -D__TRACE_FREQ__=1
endif

ifdef CONFIG_TRACE_ALLOC
PULP_CFLAGS += -D__TRACE_ALLOC__=1
endif

ifdef CONFIG_TRACE_INIT
PULP_CFLAGS += -D__TRACE_INIT__=1
endif

ifdef CONFIG_TRACE_UART
PULP_CFLAGS += -D__TRACE_UART__=1
endif


#
# RUNNER
#

ifdef RUNNER_CONFIG
override runner_args += --config-user=$(RUNNER_CONFIG)
endif

ifeq '$(load_mode)' 'fast_debug'
LOAD_MODE := FAST_DEBUG_PRELOAD
else ifeq '$(load_mode)' 'standalone'
LOAD_MODE := STANDALONE
else ifeq '$(load_mode)' 'jtag'
LOAD_MODE := JTAG
else
LOAD_MODE := JTAG
endif

ifeq '$(pulp_chip)' 'carfield-cluster'
ENTRY=0x78008080
else
ENTRY=0x1c008080
endif
#
# VSIM Flags
#
vsim_flags ?= +ENTRY_POINT=ENTRY -permit_unmatched_virtual_intf -gBAUDRATE=115200

ifdef CONFIG_PLUSARG_SIM

ifdef bootmode
ifeq ($(bootmode), spi)
vsim_flags += +bootmode=spi_flash
else ifeq ($(bootmode), hyperflash)
vsim_flags += +bootmode=hyper_flash
else ifeq ($(bootmode), fast_debug)
vsim_flags += +bootmode=fast_debug_preload
else ifeq ($(bootmode), jtag)
vsim_flags += +bootmode=jtag
else
$(error Illegal value supplied for bootmode. Legal values are 'spi', 'hyperflash', 'fast_debug' and 'jtag')
endif
else
# default bootmode
vsim_flags += +bootmode=jtag
endif

else

ifdef bootmode
ifeq ($(bootmode), spi)
vsim_flags += -gSTIM_FROM=SPI_FLASH -gLOAD_L2=STANDALONE -gUSE_S25FS256S_MODEL=1
else
ifeq ($(bootmode), hyperflash)
vsim_flags += -gSTIM_FROM=HYPER_FLASH -gLOAD_L2=STANDALONE -gUSE_HYPER_MODELS=1
else
ifeq ($(bootmode), fast_debug)
vsim_flags += -gLOAD_L2=FAST_DEBUG_PRELOAD
else
ifeq ($(bootmode), jtag)
vsim_flags += -gLOAD_L2=JTAG
else
$(error Illegal value supplied for bootmode. Legal values are 'spi', 'hyperflash', 'fast_debug' and 'jtag')
endif
endif
endif
endif
else
vsim_flags += -gLOAD_L2=JTAG
endif
endif


ifdef vsim_additional_flags
vsim_flags += $(vsim_additional_flags)
endif


#
# PULP_APPS
#

define declare_app

$(eval PULP_APP_SRCS_$(1) += $(PULP_APP_SRCS) $(PULP_APP_FC_SRCS) $(PULP_SRCS) $(PULP_APP_CL_SRCS) $(PULP_CL_SRCS))
$(eval PULP_APP_ASM_SRCS_$(1) += $(PULP_APP_ASM_SRCS) $(PULP_ASM_SRCS) $(PULP_APP_CL_ASM_SRCS) $(PULP_CL_ASM_SRCS))
$(eval PULP_APP_OBJS_$(1) += $(patsubst %.c,$(TARGET_BUILD_DIR)/$(1)/%.o,$(PULP_APP_SRCS_$(1))))
$(eval PULP_APP_OBJS_$(1) += $(patsubst %.S,$(TARGET_BUILD_DIR)/$(1)/%.o,$(PULP_APP_ASM_SRCS_$(1))))

$(eval PULP_APP_CFLAGS_$(1) += $(PULP_ARCH_CFLAGS) $(PULP_CFLAGS) $(PULP_APP_CFLAGS))
$(eval PULP_APP_LDFLAGS_$(1) += $(PULP_ARCH_LDFLAGS) $(PULP_LDFLAGS) $(PULP_APP_LDFLAGS))

-include $(PULP_APP_OBJS_$(1):.o=.d)

$(TARGET_BUILD_DIR)/$(1)/%.o: %.c
	@echo "CC  $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $$< -o $$@ -MMD -MP $(PULP_APP_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/$(1)/%.o: %.cpp
	@echo "CXX $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $< -o $@ -MMD -MP $(PULP_APP_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/$(1)/%.o: %.S
	@echo "CC  $$<"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_CC) -c $$< -o $$@ -MMD -MP -DLANGUAGE_ASSEMBLY $(PULP_APP_CFLAGS_$(1))

$(TARGET_BUILD_DIR)/$(1)/$(1): $(PULP_APP_OBJS_$(1))
	@echo "LD  $$@"
	$(V)mkdir -p `dirname $$@`
	$(V)$(PULP_LD) -o $$@ $$^ -MMD -MP $(PULP_APP_LDFLAGS_$(1))

$(TARGET_INSTALL_DIR)/bin/$(1): $(TARGET_BUILD_DIR)/$(1)/$(1)
	@echo "CP  $$@"
	$(V)mkdir -p `dirname $$@`
	$(V)cp $$< $$@

TARGETS += $(TARGET_BUILD_DIR)/$(1)/$(1)
INSTALL_TARGETS += $(TARGET_INSTALL_DIR)/bin/$(1)

endef




ifdef PULP_APP
PULP_APPS += $(PULP_APP)
endif

$(foreach app, $(PULP_APPS), $(eval $(call declare_app,$(app))))

conf:

all: $(TARGETS)

.PHONY:clean
clean:
	@echo "RM  $(TARGET_BUILD_DIR)"
	$(V)rm -rf $(TARGET_BUILD_DIR)

.PHONY: run
ifeq '$(platform)' 'gvsoc'
run:
	pulp-run --platform=$(platform) --config=$(PULPRUN_TARGET) --dir=$(TARGET_BUILD_DIR) --binary=$(TARGETS) $(runner_args) prepare run
endif

ifeq '$(platform)' 'rtl'

$(TARGET_BUILD_DIR)/modelsim.ini:
ifndef VSIM_PATH
	$(error "VSIM_PATH is undefined. Either call \
	'source $$YOUR_HW_DIR/setup/vsim.sh' or set it manually.")
endif
	ln -sfn $(VSIM_PATH)/modelsim.ini $@

$(TARGET_BUILD_DIR)/work:
ifndef VSIM_PATH
	$(error "VSIM_PATH is undefined. Either call \
	'source $$YOUR_HW_DIR/setup/vsim.sh' or set it manually.")
endif
	ln -sfn $(VSIM_PATH)/work $@

$(TARGET_BUILD_DIR)/boot:
ifndef VSIM_PATH
	$(error "VSIM_PATH is undefined. Either call \
	'source $$YOUR_HW_DIR/setup/vsim.sh' or set it manually.")
endif
	ln -sfn $(VSIM_PATH)/boot $@

$(TARGET_BUILD_DIR)/tcl_files:
ifndef VSIM_PATH
	$(error "VSIM_PATH is undefined. Either call \
	'source $$YOUR_HW_DIR/setup/vsim.sh' or set it manually.")
endif
	ln -sfn $(VSIM_PATH)/tcl_files $@

$(TARGET_BUILD_DIR)/waves:
ifndef VSIM_PATH
	$(error "VSIM_PATH is undefined. Either call \
	'source $$YOUR_HW_DIR/setup/vsim.sh' or set it manually.")
endif
	ln -sfn $(VSIM_PATH)/waves $@

$(TARGET_BUILD_DIR)/stdout:
	mkdir -p $@

$(TARGET_BUILD_DIR)/fs:
	mkdir -p $@


run: $(TARGET_BUILD_DIR)/modelsim.ini $(TARGET_BUILD_DIR)/work  $(TARGET_BUILD_DIR)/boot $(TARGET_BUILD_DIR)/tcl_files $(TARGET_BUILD_DIR)/stdout $(TARGET_BUILD_DIR)/fs $(TARGET_BUILD_DIR)/waves
	$(PULPRT_HOME)/bin/stim_utils.py --binary=$(TARGETS) --vectors=$(TARGET_BUILD_DIR)/vectors/stim.txt
	$(PULPRT_HOME)/bin/plp_mkflash  --flash-boot-binary=$(TARGETS)  --stimuli=$(TARGET_BUILD_DIR)/vectors/qspi_stim.slm --flash-type=spi --qpi
	$(PULPRT_HOME)/bin/slm_hyper.py  --input=$(TARGET_BUILD_DIR)/vectors/qspi_stim.slm  --output=$(TARGET_BUILD_DIR)/vectors/hyper_stim.slm
ifndef VSIM_PATH
	$(error "VSIM_PATH is undefined. Either call \
	'source $$YOUR_HW_DIR/setup/vsim.sh' or set it manually.")
endif

ifdef gui
	cd $(TARGET_BUILD_DIR) && export VSIM_RUNNER_FLAGS='$(vsim_flags)' && export VOPT_ACC_ENA="YES" && $(VSIM) -64 -do 'source $(VSIM_PATH)/tcl_files/config/run_and_exit.tcl' -do 'source $(VSIM_PATH)/tcl_files/run.tcl; '
else
	cd $(TARGET_BUILD_DIR) && export VSIM_RUNNER_FLAGS='$(vsim_flags)' && $(VSIM) -64 -c -do 'source $(VSIM_PATH)/tcl_files/config/run_and_exit.tcl' -do 'source $(VSIM_PATH)/tcl_files/run.tcl; run_and_exit;'
endif

endif

ifeq '$(platform)' 'fpga'
run:
	$(PULPRT_HOME)/bin/elf_run_genesys2.sh $(TARGETS)
endif

dis:
	$(PULP_OBJDUMP) $(PULP_ARCH_OBJDFLAGS) $(disopt) $(TARGETS)

size:
	$(PULPRT_HOME)/bin/pos-size --binary=$(TARGETS) --depth=10

help:
	@echo "Makefile options:"
	@echo "  CONFIG_TRACE_LEVEL=<level>    Activate traces for the specified level (0=none, 1=fatal, 2=error, 3=warning, 4=info, 5=debug, 6=trace)."
	@echo "  CONFIG_TRACE_ALL=1            Activate all traces. Other traces can be individually activated with CONFIG_TRACE_<NAME>."
