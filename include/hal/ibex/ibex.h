/*
 * Copyright (C) 2020 ETH Zurich and University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __HAL_IBEX_IBEX_H__
#define __HAL_IBEX_IBEX_H__

#include "archi/pulp.h"

#include "archi/riscv/builtins_v2_emu.h"

// For PULP, ibex added non-standard irqs to allow for 32 fast interrupts. 
// These use custom CSRs: MIE: 0x7D0, MTVEC: 0x7D1, MIP: 0x7D2
#define SR_MTVEC  0x7D1

#define hal_spr_read_then_clr(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrc %0, %1, %2" :  "=r" (state) : "I" (reg), "I" (val) ); \
    state; \
  })

#define hal_spr_read_then_set(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrs %0, %1, %2" :  "=r" (state) : "I" (reg), "I" (val) ); \
    state; \
  })

#define hal_spr_read_then_clr_from_reg(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrc %0, %1, %2" :  "=r" (state) : "I" (reg), "r" (val) ); \
    state; \
  })

#define hal_spr_read_then_set(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrs %0, %1, %2" :  "=r" (state) : "I" (reg), "I" (val) ); \
    state; \
  })

#define hal_spr_read_then_set_from_reg(reg,val) \
  ({ \
    int state; \
    asm volatile ("csrrs %0, %1, %2" :  "=r" (state) : "I" (reg), "r" (val) ); \
    state; \
  })

#define hal_spr_write(reg,val) \
({do { \
  asm volatile ("csrw %0, %1" :  : "I" (reg), "r" (val) ); \
} while(0); \
})

#define hal_spr_read(reg) \
({ \
  int result; \
  asm volatile ("csrr %0, %1" : "=r" (result) : "I" (reg) ); \
  result; \
})

#define hal_mepc_read() hal_spr_read(RV_CSR_MEPC)

static inline unsigned int core_id() {
  int hart_id;
  asm("csrr %0, 0xF14" : "=r" (hart_id) : );
  // in PULP the hart id is {22'b0, cluster_id, core_id}
  return hart_id & 0x01f;
}

static inline unsigned int cluster_id() {
  int hart_id;
  asm("csrr %0, 0xF14" : "=r" (hart_id) : );
  // in PULP the hart id is {22'b0, cluster_id, core_id}
  return (hart_id >> 5) & 0x3f;
}

static inline unsigned int hal_core_id() {
  return core_id();
}

static inline unsigned int hal_cluster_id() {
  return cluster_id();
}

// TODO replace by compiler builtin
static inline __attribute__((always_inline)) unsigned int hal_has_fc() {
#ifdef ARCHI_HAS_FC
  return 1;
#else
  return 0;
#endif
}

static inline __attribute__((always_inline)) unsigned int hal_is_fc() {
#ifndef ARCHI_HAS_FC
  return 0;
#else
  if (hal_has_fc()) return hal_cluster_id() == ARCHI_FC_CID;
  else return 0;
#endif
}

static inline int hal_irq_disable()
{
  int irq = hal_spr_read_then_clr(0x300, 0x1<<3);
  // This memory barrier is needed to prevent the compiler to cross the irq barrier
  __asm__ __volatile__ ("" : : : "memory");
  return irq;
}

static inline void hal_irq_restore(int state)
{
  // This memory barrier is needed to prevent the compiler to cross the irq barrier
  __asm__ __volatile__ ("" : : : "memory");
  hal_spr_write(0x300, state);
}

static inline void hal_irq_enable()
{
  // This memory barrier is needed to prevent the compiler to cross the irq barrier
  __asm__ __volatile__ ("" : : : "memory");
  hal_spr_read_then_set(0x300, 0x1<<3);
}


/*
 * PERFORMANCE COUNTERS
 * 
 * API for accessing performance counters registers.
 * Have a look at file mhpm.h to speficy registers through defines
 * CSR_PCER_* and CSR_PCMR_*
 */

#define PCER_NB_EVENTS CSR_PCER_NB_EVENTS
#define PCER_ALL_EVENTS_MASK CSR_PCER_ALL_EVENTS_MASK

/* Configure the active events. eventMask is an OR of events got through SPR_PCER_EVENT_MASK */
static inline void cpu_perf_conf_events(unsigned int eventMask)
{
#ifndef PLP_NO_PERF_COUNTERS
  unsigned int test = ~eventMask;
  asm volatile ("csrw 0x320, %0" : "+r" (test));
#endif
}

/* Return events configuration */
static inline unsigned int cpu_perf_conf_events_get()
{
#ifndef PLP_NO_PERF_COUNTERS
  unsigned int result;
  asm volatile ("csrr %0, 0x320" : "=r" (result));
  return ~result;
#else
  return 0;
#endif
}

/* Configure the mode. confMask is an OR of all SPR_PCMR_* macros */
static inline void cpu_perf_conf(unsigned int confMask)
{
#ifndef PLP_NO_PERF_COUNTERS
  asm volatile ("csrw 0x320, %0" :: "r" (~confMask));
#endif
}

/* Starts counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_start() {
#ifndef PLP_NO_PERF_COUNTERS
  cpu_perf_conf(CSR_PCER_ALL_EVENTS_MASK);
#endif
}

/* Stops counting in all counters. As this is using the mode register,
 * the rest of the config can be given through conf parameter */
static inline void cpu_perf_stop() {
#ifndef PLP_NO_PERF_COUNTERS
  cpu_perf_conf(0);
#endif
}

/* Set the specified counter to the specified value */
static inline void cpu_perf_set(unsigned int counterId, unsigned int value) {
  switch(counterId) {
    case  0: asm volatile ("csrw 0xB00, %0" :  : "r" (value) ); break;
    case  1: break; // This CSR does not exist
    case  2: asm volatile ("csrw 0xB02, %0" :  : "r" (value) ); break;
    case  3: asm volatile ("csrw 0xB03, %0" :  : "r" (value) ); break;
    case  4: asm volatile ("csrw 0xB04, %0" :  : "r" (value) ); break;
    case  5: asm volatile ("csrw 0xB05, %0" :  : "r" (value) ); break;
    case  6: asm volatile ("csrw 0xB06, %0" :  : "r" (value) ); break;
    case  7: asm volatile ("csrw 0xB07, %0" :  : "r" (value) ); break;
    case  8: asm volatile ("csrw 0xB08, %0" :  : "r" (value) ); break;
    case  9: asm volatile ("csrw 0xB09, %0" :  : "r" (value) ); break;
    case 10: asm volatile ("csrw 0xB0A, %0" :  : "r" (value) ); break;
    case 11: asm volatile ("csrw 0xB0B, %0" :  : "r" (value) ); break;
    case 12: asm volatile ("csrw 0xB0C, %0" :  : "r" (value) ); break;
    case 13: asm volatile ("csrw 0xB0D, %0" :  : "r" (value) ); break;
    case 14: asm volatile ("csrw 0xB0E, %0" :  : "r" (value) ); break;
    case 15: asm volatile ("csrw 0xB0F, %0" :  : "r" (value) ); break;
    case 16: asm volatile ("csrw 0xB10, %0" :  : "r" (value) ); break;
    case 17: asm volatile ("csrw 0xB11, %0" :  : "r" (value) ); break;
    case 18: asm volatile ("csrw 0xB12, %0" :  : "r" (value) ); break;
    case 19: asm volatile ("csrw 0xB13, %0" :  : "r" (value) ); break;
    case 20: asm volatile ("csrw 0xB14, %0" :  : "r" (value) ); break;
    case 21: asm volatile ("csrw 0xB15, %0" :  : "r" (value) ); break;
    case 22: asm volatile ("csrw 0xB16, %0" :  : "r" (value) ); break;
    case 23: asm volatile ("csrw 0xB17, %0" :  : "r" (value) ); break;
    case 24: asm volatile ("csrw 0xB18, %0" :  : "r" (value) ); break;
    case 25: asm volatile ("csrw 0xB19, %0" :  : "r" (value) ); break;
    case 26: asm volatile ("csrw 0xB1A, %0" :  : "r" (value) ); break;
    case 27: asm volatile ("csrw 0xB1B, %0" :  : "r" (value) ); break;
    case 28: asm volatile ("csrw 0xB1C, %0" :  : "r" (value) ); break;
    case 29: asm volatile ("csrw 0xB1D, %0" :  : "r" (value) ); break;
    case 30: asm volatile ("csrw 0xB1E, %0" :  : "r" (value) ); break;
    case 31: asm volatile ("csrw 0xB1F, %0" :  : "r" (value) ); break;
  }
}

/* Set all counters to the specified value */
static inline void cpu_perf_setall(unsigned int value) {
#ifndef PLP_NO_PERF_COUNTERS
  // This implementation is rather slow. ri5cy has a register to set all, ibex does not.
  for (int i = 0; i < CSR_PCER_TOP_EVENT; i++) {
    cpu_perf_set(i, value);
  }
#endif
}

/* Return the value of the specified counter */
static inline unsigned int cpu_perf_get(const unsigned int counterId) {
#ifndef PLP_NO_PERF_COUNTERS
  unsigned int value; 
  switch(counterId) {
    case  0: asm volatile ("csrr %0, 0xB00" : "=r" (value)); break;
    case  1: break; // This CSR does not exist
    case  2: asm volatile ("csrr %0, 0xB02" : "=r" (value)); break;
    case  3: asm volatile ("csrr %0, 0xB03" : "=r" (value)); break;
    case  4: asm volatile ("csrr %0, 0xB04" : "=r" (value)); break;
    case  5: asm volatile ("csrr %0, 0xB05" : "=r" (value)); break;
    case  6: asm volatile ("csrr %0, 0xB06" : "=r" (value)); break;
    case  7: asm volatile ("csrr %0, 0xB07" : "=r" (value)); break;
    case  8: asm volatile ("csrr %0, 0xB08" : "=r" (value)); break;
    case  9: asm volatile ("csrr %0, 0xB09" : "=r" (value)); break;
    case 10: asm volatile ("csrr %0, 0xB0A" : "=r" (value)); break;
    case 11: asm volatile ("csrr %0, 0xB0B" : "=r" (value)); break;
    case 12: asm volatile ("csrr %0, 0xB0C" : "=r" (value)); break;
    case 13: asm volatile ("csrr %0, 0xB0D" : "=r" (value)); break;
    case 14: asm volatile ("csrr %0, 0xB0E" : "=r" (value)); break;
    case 15: asm volatile ("csrr %0, 0xB0F" : "=r" (value)); break;
    case 16: asm volatile ("csrr %0, 0xB10" : "=r" (value)); break;
    case 17: asm volatile ("csrr %0, 0xB11" : "=r" (value)); break;
    case 18: asm volatile ("csrr %0, 0xB12" : "=r" (value)); break;
    case 19: asm volatile ("csrr %0, 0xB13" : "=r" (value)); break;
    case 20: asm volatile ("csrr %0, 0xB14" : "=r" (value)); break;
    case 21: asm volatile ("csrr %0, 0xB15" : "=r" (value)); break;
    case 22: asm volatile ("csrr %0, 0xB16" : "=r" (value)); break;
    case 23: asm volatile ("csrr %0, 0xB17" : "=r" (value)); break;
    case 24: asm volatile ("csrr %0, 0xB18" : "=r" (value)); break;
    case 25: asm volatile ("csrr %0, 0xB19" : "=r" (value)); break;
    case 26: asm volatile ("csrr %0, 0xB1A" : "=r" (value)); break;
    case 27: asm volatile ("csrr %0, 0xB1B" : "=r" (value)); break;
    case 28: asm volatile ("csrr %0, 0xB1C" : "=r" (value)); break;
    case 29: asm volatile ("csrr %0, 0xB1D" : "=r" (value)); break;
    case 30: asm volatile ("csrr %0, 0xB1E" : "=r" (value)); break;
    case 31: asm volatile ("csrr %0, 0xB1F" : "=r" (value)); break;
  }
  return value;
#else
  return 0;
#endif
}

static inline const char *cpu_perf_name(int event) {
  return CSR_PCER_NAME(event);
}

#endif
