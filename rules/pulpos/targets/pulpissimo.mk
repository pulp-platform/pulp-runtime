ifdef USE_CV32E40P
PULP_LDFLAGS      +=
PULP_CFLAGS       +=  -D__cv32e40p__ -U__riscv__
PULP_ARCH_CFLAGS  ?=  -march=rv32imfc_xcvalu_xcvbi_xcvbitmanip_xcvhwlp_xcvmac_xcvmem_xcvsimd_xcvelw_zfhmin -mabi=ilp32f
PULP_ARCH_LDFLAGS ?=  -march=rv32imfc_xcvalu_xcvbi_xcvbitmanip_xcvhwlp_xcvmac_xcvmem_xcvsimd_xcvelw_zfhmin -mabi=ilp32f
PULP_ARCH_OBJDFLAGS ?=
else
PULP_LDFLAGS      +=
PULP_CFLAGS       +=  -D__riscv__
PULP_ARCH_CFLAGS  ?=  -march=rv32imcxgap9
PULP_ARCH_LDFLAGS ?=  -march=rv32imcxgap9
PULP_ARCH_OBJDFLAGS ?= -Mmarch=rv32imcxgap9
endif

PULP_CFLAGS    += -fdata-sections -ffunction-sections -include chips/pulpissimo/config.h -I$(PULPRT_HOME)/include/chips/pulpissimo
PULP_OMP_CFLAGS    += -fopenmp -mnativeomp
PULP_LDFLAGS += -nostartfiles -nostdlib -Wl,--gc-sections -L$(PULPRT_HOME)/kernel -Tchips/pulpissimo/link.ld -lgcc

ifdef USE_CV32E40P
PULP_CC      = riscv64-unknown-elf-gcc
PULP_AR     ?= riscv64-unknown-elf-ar
PULP_LD     ?= riscv64-unknown-elf-gcc
PULP_OBJDUMP ?= riscv64-unknown-elf-objdump
# CORE-V march doesn't match any multilib, so the toolchain defaults to the
# 64-bit root libgcc.a. Explicitly add the rv32imafc/ilp32f multilib dir which
# has the correct 32-bit runtime helpers (e.g. __clzsi2).
_CV32_TOOLCHAIN_ROOT := $(or $(PULP_RUNTIME_GCC_TOOLCHAIN),$(PULP_RISCV_GCC_TOOLCHAIN))
_CV32_LIBGCC_DIR := $(firstword $(wildcard $(_CV32_TOOLCHAIN_ROOT)/lib/gcc/riscv64-unknown-elf/*/rv32imafc/ilp32f))
PULP_LDFLAGS += -L$(_CV32_LIBGCC_DIR)
else
PULP_CC      = riscv32-unknown-elf-gcc
PULP_AR     ?= riscv32-unknown-elf-ar
PULP_LD     ?= riscv32-unknown-elf-gcc
PULP_OBJDUMP ?= riscv32-unknown-elf-objdump
endif

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


include $(PULPRT_HOME)/rules/pulpos/configs/default.mk
include $(PULPRT_HOME)/rules/pulpos/default_rules.mk