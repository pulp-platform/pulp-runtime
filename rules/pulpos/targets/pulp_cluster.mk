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
PULP_ARCH_CFLAGS ?=  -march=rv32imcxpulpnn
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxpulpnn
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxpulpnn
endif

ifndef LEGACY_DMA
PULP_CFLAGS    += -DUSE_IDMA
endif
PULP_CFLAGS    += -fdata-sections -ffunction-sections -include chips/pulp_cluster/config.h -I$(PULPRT_HOME)/include/chips/pulp_cluster
PULP_OMP_CFLAGS    += -fopenmp -mnativeomp
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/pulp_cluster/link.ld -lgcc

PULP_CC = riscv32-unknown-elf-gcc 
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump

fc/archi=riscv
pe/archi=riscv
pulp_chip=pulp_cluster
pulp_chip_family=pulp_cluster
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
PULP_SRCS     += kernel/chips/pulp_cluster/soc.c


include $(PULPRT_HOME)/rules/pulpos/configs/default.mk

ifeq '$(platform)' 'fpga'
CONFIG_IO_UART=1
endif

include $(PULPRT_HOME)/rules/pulpos/default_rules.mk

ifndef gui
vsim-flags = -c
endif


VSIM ?= vsim 

run:
ifdef gui
	$(VSIM) $(vsim-flags) -do "set  VSIM_PATH $(VSIM_PATH); set  APP $(TARGET_BUILD_DIR)/$(PULP_APP)/$(PULP_APP); source $(VSIM_PATH)/scripts/start.tcl"
else
	$(VSIM) $(vsim-flags) -do "set  VSIM_PATH $(VSIM_PATH); set  APP $(TARGET_BUILD_DIR)/$(PULP_APP)/$(PULP_APP); source $(VSIM_PATH)/scripts/run_and_exit.tcl"
endif
