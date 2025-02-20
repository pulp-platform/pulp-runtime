HOSTNAME := $(shell hostname)
ETH_HOST = $(shell echo $(HOSTNAME) | grep -q "\.ee\.ethz\.ch$$" && echo 1 || echo 0)
ifeq ($(ETH_HOST),1)
QUESTA ?= questa-2022.3
else
QUESTA ?= 
endif
ifdef USE_IBEX
PULP_LDFLAGS      +=
PULP_CFLAGS       +=  -D__ibex__ -U__riscv__ -UARCHI_CORE_HAS_PULPV2 -DRV_ISA_RV32
PULP_ARCH_CFLAGS ?=  -march=rv32imc
PULP_ARCH_LDFLAGS ?=  -march=rv32imc
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imc
else ifdef USE_CV32E40P
PULP_LDFLAGS +=
PULP_CFLAGS += -D__cv32e40p__ -U__riscv__ -UARCHI_CORE_HAS_PULPV2
PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9 -mnohwloop
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9 -mnohwloop
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9 -mnohwloop
else
PULP_LDFLAGS      +=
PULP_CFLAGS       +=  -D__riscv__
PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
endif

PULP_CFLAGS    += -fdata-sections -ffunction-sections -include chips/astral-cluster/config.h -I$(PULPRT_HOME)/include/chips/astral-cluster
PULP_OMP_CFLAGS    += -fopenmp -mnativeomp
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/astral-cluster/link.ld -lgcc

PULPD_RISCV ?= riscv32-unknown-elf

PULP_CC ?= $(PULPD_RISCV)-gcc
PULP_AR ?= $(PULPD_RISCV)-ar
PULP_LD ?= $(PULPD_RISCV)-gcc
PULP_OBJDUMP ?= $(PULPD_RISCV)-objdump

fc/archi=riscv
pe/archi=riscv
pulp_chip=astral-cluster
pulp_chip_family=astral-cluster
cluster/version=5
fc_itc/version=1
udma/cpi/version=1
udma/i2c/version=2
soc/fll/version=1
udma/i2s/version=2
udma/uart/version=1
event_unit/version=3
perf_counters=True
fll/version=1
padframe/version=1
udma/spim/version=3
gpio/version=3
udma/archi=3
udma/version=3
soc_eu/version=2


# FLL
PULP_SRCS     += kernel/fll-v$(fll/version).c
PULP_SRCS     += kernel/freq-domains.c
PULP_SRCS     += kernel/chips/astral-cluster/soc.c

# HMR
ifdef ARCHI_HMR
PULP_CFLAGS += -DARCHI_HMR
PULP_SRCS += kernel/hmr_synch.c
endif

# ECC
ifdef NO_ECC
PULP_CFLAGS += -DNO_ECC
endif

include $(PULPRT_HOME)/rules/pulpos/configs/default.mk

ifeq '$(platform)' 'fpga'
CONFIG_IO_UART=1
endif

include $(PULPRT_HOME)/rules/pulpos/default_rules.mk

ifndef gui
vsim-flags = -c
endif

run: $(TARGETS)
	cd $(TARGET_BUILD_DIR); $(QUESTA) vsim $(vsim-flags) -do "set  VSIM_PATH $(VSIM_PATH); source $(VSIM_PATH)/scripts/start.tcl"
