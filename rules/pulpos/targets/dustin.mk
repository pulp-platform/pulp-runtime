PULP_LDFLAGS      += 
PULP_CFLAGS       +=  -D__riscv__
PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
PULP_CFLAGS    += -fdata-sections -ffunction-sections -include chips/dustin/config.h -I$(PULPRT_HOME)/include/chips/dustin
PULP_OMP_CFLAGS    += -fopenmp -mnativeomp
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/dustin/link.ld -lgcc

PULP_CC = riscv32-unknown-elf-gcc 
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump

fc/archi=riscv
pe/archi=riscv
pulp_chip=dustin
pulp_chip_family=dustin
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
PULP_SRCS     += kernel/chips/dustin/soc.c


include $(PULPRT_HOME)/rules/pulpos/configs/default.mk

ifeq '$(platform)' 'fpga'
CONFIG_IO_UART=1
endif

include $(PULPRT_HOME)/rules/pulpos/default_rules.mk
