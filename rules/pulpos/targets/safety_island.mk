
PULP_LDFLAGS +=
PULP_CFLAGS += -D__cv32e40p__ -U__riscv__ -UARCHI_CORE_HAS_PULPV2

PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9
PULP_ARCH_OBJDFLAGS ?= -march=riscv32imcxgap9
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9

# PULP_ARCH_CFLAGS ?=  -march=rv32imc_zfinx_xcorev -mno-nohwloop
# PULP_ARCH_LDFLAGS ?=  -march=rv32imc_zfinx_xcorev -mno-nohwloop
# PULP_ARCH_OBJDFLAGS ?=

PULP_CFLAGS    += -fdata-sections -ffunction-sections -include chips/safety-island/config.h -I$(PULPRT_HOME)/include/chips/safety-island
PULP_OMP_CFLAGS    += -fopenmp -mnativeomp
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/safety-island/link.ld -lgcc

PULP_CC = riscv32-unknown-elf-gcc
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump

fc/archi=riscv
pe/archi=riscv
pulp_chip=safety-island
pulp_chip_family=safety-island

PULP_SRCS     += kernel/chips/safety-island/soc.c


include $(PULPRT_HOME)/rules/pulpos/configs/default.mk

ifeq '$(platform)' 'fpga'
CONFIG_IO_UART=1
endif

include $(PULPRT_HOME)/rules/pulpos/default_rules.mk
