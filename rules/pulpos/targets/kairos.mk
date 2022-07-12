ifdef USE_IBEX
$(error IBEX is not supported in control-pulp)
endif

# we need at least pulp-gcc v2.4.0
PULP_LDFLAGS      +=
PULP_CFLAGS       +=  -D__cv32e40p__ -D__riscv__
PULP_ARCH_CFLAGS ?=  -march=rv32imc_zfinx_xcorev -mabi=ilp32 -mno-pulp-hwloop
PULP_ARCH_LDFLAGS ?=  -march=rv32imc_zfinx_xcorev -mabi=ilp32 -mno-pulp-hwloop
# uses elf attributes to disassemble so no need to set it manually
PULP_ARCH_OBJDFLAGS ?=

PULP_CFLAGS    += -fdata-sections -ffunction-sections \
	-include chips/kairos/config.h -I$(PULPRT_HOME)/include/chips/kairos
PULP_OMP_CFLAGS    += -fopenmp -mnativeomp
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections \
	-L$(PULPRT_HOME)/kernel -Tchips/kairos/link.ld -lgcc

PULP_CC = riscv32-unknown-elf-gcc
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump
PULP_OBJCOPY ?= riscv32-unknown-elf-objcopy

fc/archi=riscv
pe/archi=riscv
pulp_chip=kairos
pulp_chip_family=kairos
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
PULP_SRCS     += kernel/chips/kairos/soc.c


include $(PULPRT_HOME)/rules/pulpos/configs/default.mk

ifeq '$(platform)' 'fpga'
CONFIG_IO_UART=1
endif

include $(PULPRT_HOME)/rules/pulpos/default_rules.mk
