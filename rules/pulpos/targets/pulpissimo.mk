ifdef USE_IBEX
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/pulpissimo/link.ld -lgcc
PULP_CFLAGS += -D__ibex__ -D__riscv__ -UARCHI_CORE_HAS_PULPV2 -DRV_ISA_RV32
PULP_ARCH_CFLAGS ?= -march=rv32imc
PULP_ARCH_LDFLAGS ?= -march=rv32imc
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imc
else ifdef USE_CV32E40P
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/pulpissimo/link.ld -lgcc
PULP_CFLAGS += -D__cv32e40p__ -D__riscv__ -UARCHI_CORE_HAS_PULPV2
ifdef CONFIG_USE_ZFINX
PULP_ARCH_CFLAGS ?=  -march=rv32imc_zfinx_xcorev -mno-pulp-hwloop
PULP_ARCH_LDFLAGS ?=  -march=rv32imc_zfinx_xcorev -mno-pulp-hwloop
else
PULP_ARCH_CFLAGS ?=  -march=rv32imfc_xcorev -mno-pulp-hwloop
PULP_ARCH_LDFLAGS ?=  -march=rv32imfc_xcorev -mno-pulp-hwloop
endif
PULP_ARCH_OBJDFLAGS ?=
else ifdef USE_CV32E40X
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/pulpissimo/link.ld
PULP_CFLAGS       +=  -D__riscv__ -UARCHI_CORE_HAS_PULPV2 -DRV_ISA_RV32 -DPULP_LLVM
PULP_ARCH_CFLAGS ?=  -march=rv32imcxfir --target=riscv32
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxfir --target=riscv32
PULP_ARCH_OBJDFLAGS ?= -D
# use LLVM for CV32E40X
PULP_CC = clang
PULP_AR = llvm-ar
PULP_LD = clang
PULP_OBJDUMP = llvm-objdump
else
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/pulpissimo/link.ld -lgcc
PULP_CFLAGS       +=  -D__riscv__
PULP_ARCH_CFLAGS ?=  -march=rv32imcxgap9
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
endif

# without -fno-tree-loop-distribute-patterns, gcc would insert memset & co where it shouldn't! only avoid this option in CV32E40X+LLVM
ifdef USE_CV32E40X
PULP_CFLAGS    += -fdata-sections -ffunction-sections -include chips/pulpissimo/config.h -I$(PULPRT_HOME)/include/chips/pulpissimo
else
PULP_CFLAGS    += -fno-tree-loop-distribute-patterns  -fdata-sections -ffunction-sections -include chips/pulpissimo/config.h -I$(PULPRT_HOME)/include/chips/pulpissimo
endif
PULP_OMP_CFLAGS    += -fopenmp -mnativeomp

PULP_CC ?= riscv32-unknown-elf-gcc
PULP_AR ?= riscv32-unknown-elf-ar
PULP_LD ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump

fc/archi=riscv
pe/archi=riscv
pulp_chip=pulp
pulp_chip_family=pulp
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
PULP_SRCS     += kernel/chips/pulpissimo/soc.c

# GPIO
PULP_SRCS     += drivers/gpio/gpio.c
PULP_CFLAGS += -I$(PULPRT_HOME)/drivers/gpio/include

# Pad Multiplexing
ifeq '$(platform)' 'rtl'
PULP_SRCS += drivers/pulpissimo/rtl_sim/io_mux/src/io_mux.c
PULP_CFLAGS += -I$(PULPRT_HOME)/drivers/pulpissimo/rtl_sim/io_mux/include
endif
ifeq '$(platform)' 'verilator'
PULP_SRCS += drivers/pulpissimo/rtl_sim/io_mux/src/io_mux.c
PULP_CFLAGS += -I$(PULPRT_HOME)/drivers/pulpissimo/rtl_sim/io_mux/include
endif

include $(PULPRT_HOME)/rules/pulpos/configs/default.mk

ifeq '$(platform)' 'fpga'
CONFIG_IO_UART=1
endif

include $(PULPRT_HOME)/rules/pulpos/default_rules.mk
