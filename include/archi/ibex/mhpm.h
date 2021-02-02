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

#ifndef _ARCHI_IBEX_MHPM_H
#define _ARCHI_IBEX_MHPM_H

/*
 * Bit definitions for Performance counters mode registers
 * (see https://ibex-core.readthedocs.io/en/latest/03_reference/performance_counters.html)
 */

#define IBEX_MHPMCOUNTER_BASE 0xB00
// Currently the additional register for more perf. counting is not yet implemented

// ibex performance counters
#define IBEX_NUM_CYCLES          0  /* NumCycles: Number of cycles */
#define IBEX_NUM_INSTR_RET       2  /* NumInstrRet: Number of instructions retired */
#define IBEX_NUM_CYCLES_LSU      3  /* NumCyclesLSU: Number of cycles waiting for data memory */
#define IBEX_NUM_CYCLES_IF       4  /* NumCyclesIF: Cycles waiting for instruction fetches, i.e., number of instructions wasted due to non-ideal caching */
#define IBEX_NUM_LOADS           5  /* NumLoads: Number of data memory loads. Misaligned accesses are counted as two accesses */
#define IBEX_NUM_STORES          6  /* NumStores: Number of data memory stores. Misaligned accesses are counted as two accesses */
#define IBEX_NUM_JUMPS           7  /* NumJumps: Number of unconditional jumps (j, jal, jr, jalr) */
#define IBEX_NUM_BRANCHES        8  /* NumBranches: Number of branches (conditional) */
#define IBEX_NUM_BRANCHES_TAKEN  9  /* NumBranchesTaken: Number of taken branches (conditional) */
#define IBEX_NUM_INSTR_RET_C     10 /* NumInstrRetC: Number of compressed instructions retired */
#define IBEX_NUM_CYCLES_MUL_WAIT 11 /* NumCyclesMulWait: Cycles waiting for multiply to complete */
#define IBEX_NUM_CYCLES_DIV_WAIT 12 /* NumCyclesDivWait: Cycles waiting for divide to complete */


// riscv map for ibex counters
#define CSR_PCER_CYCLES       IBEX_NUM_CYCLES         /* Count the number of cycles the core was running */
#define CSR_PCER_INSTR        IBEX_NUM_INSTR_RET      /* Count the number of instructions executed */
#define CSR_PCER_LD_STALL     IBEX_NUM_CYCLES_LSU     /* Number of load use hazards */
#define CSR_PCER_IMISS        IBEX_NUM_CYCLES_IF      /* Cycles waiting for instruction fetches. i.e. the number of instructions wasted due to non-ideal caches */
#define CSR_PCER_LD           IBEX_NUM_LOADS          /* Number of memory loads executed. Misaligned accesses are counted twice */
#define CSR_PCER_ST           IBEX_NUM_STORES         /* Number of memory stores executed. Misaligned accesses are counted twice */
#define CSR_PCER_JUMP         IBEX_NUM_JUMPS          /* Number of jump instructions seen, i.e. j, jr, jal, jalr */
#define CSR_PCER_BRANCH       IBEX_NUM_BRANCHES       /* Number of branch instructions seen, i.e. bf, bnf */
#define CSR_PCER_TAKEN_BRANCH IBEX_NUM_BRANCHES_TAKEN /* Number of taken branch instructions seen, i.e. bf, bnf */
#define CSR_PCER_RVC          IBEX_NUM_INSTR_RET_C    /* Number of compressed instructions */
// Not implemented in ibex
// #define CSR_PCER_ELW          0   /* Cycles wasted due to ELW instruction */
// #define CSR_PCER_JMP_STALL    0   /* Number of jump register hazards */


// External perf counters
#define IBEX_EXTERNAL_BASE    13
#define CSR_PCER_LD_EXT       13  /* Number of memory loads to EXT executed. Misaligned accesses are counted twice. Every non-TCDM access is considered external */
#define CSR_PCER_ST_EXT       14  /* Number of memory stores to EXT executed. Misaligned accesses are counted twice. Every non-TCDM access is considered external */
#define CSR_PCER_LD_EXT_CYC   15  /* Cycles used for memory loads to EXT. Every non-TCDM access is considered external */
#define CSR_PCER_ST_EXT_CYC   16  /* Cycles used for memory stores to EXT. Every non-TCDM access is considered external */
#define CSR_PCER_TCDM_CONT    17  /* Cycles wasted due to TCDM/log-interconnect contention */


#define CSR_PCER_NB_EVENTS      16
#define CSR_PCER_TOP_EVENT      17
#define CSR_PCER_NB_INTERNAL_EVENTS    11
#define CSR_PCER_TOP_INTERNAL_EVENTS    12
#define CSR_NB_PCCR             32

// Gives from the event ID, the HW mask that can be stored (with an OR with other events mask) to the PCER
#define CSR_PCER_EVENT_MASK(eventId)  (1<<(eventId))
#define CSR_PCER_ALL_EVENTS_MASK  0xffffffff

#define CSR_PCER_NAME(id) \
    ( id == IBEX_NUM_CYCLES ? "Cycles" : \
      id == IBEX_NUM_INSTR_RET ? "Instructions" : \
      id == IBEX_NUM_CYCLES_LSU ? "LD_Stall" : \
      id == IBEX_NUM_CYCLES_IF ? "IMISS" : \
      id == IBEX_NUM_LOADS ? "LD" : \
      id == IBEX_NUM_STORES ? "ST" : \
      id == IBEX_NUM_JUMPS ? "JUMP" : \
      id == IBEX_NUM_BRANCHES ? "BRANCH" : \
      id == IBEX_NUM_BRANCHES_TAKEN ? "TAKEN_BRANCH" : \
      id == IBEX_NUM_INSTR_RET_C ? "RVC" : \
      id == 16 ? "LD_EXT" : \
      id == 17 ? "ST_EXT" : \
      id == 18 ? "LD_EXT_CYC" : \
      id == 19 ? "ST_EXT_CYC" : \
      id == 20 ? "TCDM_CONT" : \
      "NA")

#endif
