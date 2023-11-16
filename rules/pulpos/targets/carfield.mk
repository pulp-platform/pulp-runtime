
PULP_LDFLAGS +=
PULP_CFLAGS += -D__cv32e40p__ -U__riscv__ -UARCHI_CORE_HAS_PULPV2

PULP_ARCH_CFLAGS    ?= -march=rv32imac_zfinx_xcorev -mabi=ilp32 -mno-pulp-hwloop
PULP_ARCH_LDFLAGS   ?= -march=rv32imac_zfinx_xcorev -mabi=ilp32 -mno-pulp-hwloop
PULP_ARCH_OBJDFLAGS ?=

PULP_CFLAGS    += -fdata-sections -ffunction-sections -include chips/carfield/config.h -I$(PULPRT_HOME)/include/chips/carfield
PULP_OMP_CFLAGS    += -fopenmp -mnativeomp
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/carfield/link.ld -lgcc

SAFED_RISCV ?= riscv32-unknown-elf

PULP_CC ?= $(SAFED_RISCV)-gcc
PULP_AR ?= $(SAFED_RISCV)-ar
PULP_LD ?= $(SAFED_RISCV)-gcc
PULP_OBJDUMP ?= $(SAFED_RISCV)-objdump

fc/archi=riscv
pe/archi=riscv
pulp_chip=carfield
pulp_chip_family=carfield

PULP_SRCS     += kernel/chips/carfield/soc.c

# HMR
PULP_CFLAGS += -DARCHI_HMR -DARCHI_HMR_FIXED
PULP_SRCS += kernel/hmr_synch.c

include $(PULPRT_HOME)/rules/pulpos/configs/default.mk

ifeq '$(platform)' 'fpga'
CONFIG_IO_UART=1
endif

include $(PULPRT_HOME)/rules/pulpos/default_rules.mk
