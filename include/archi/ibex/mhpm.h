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
 *
 */

#define IBEX_MHPMCOUNTER_BASE 0xB00
// Currently the additional register for more perf. counting is not yet implemented

// ibex performance counters
#define IBEX_NumCycles        0  /* Number of cycles */
#define IBEX_NumInstrRet      2  /* Number of instructions retired */
#define IBEX_NumCyclesLSU     3  /* Number of cycles waiting for data memory */
#define IBEX_NumCyclesIF      4  /* Cycles waiting for instruction fetches, i.e., number of instructions wasted due to non-ideal caching */
#define IBEX_NumLoads         5  /* Number of data memory loads. Misaligned accesses are counted as two accesses */
#define IBEX_NumStores        6  /* Number of data memory stores. Misaligned accesses are counted as two accesses */
#define IBEX_NumJumps         7  /* Number of unconditional jumps (j, jal, jr, jalr) */
#define IBEX_NumBranches      8  /* Number of branches (conditional) */
#define IBEX_NumBranchesTaken 9  /* Number of taken branches (conditional) */
#define IBEX_NumInstrRetC     10 /* Number of compressed instructions retired */
#define IBEX_NumCyclesMulWait 11 /* Cycles waiting for multiply to complete */
#define IBEX_NumCyclesDivWait 12 /* Cycles waiting for divide to complete */


// riscv map for ibex counters
#define CSR_PCER_CYCLES       IBEX_NumCycles        /* Count the number of cycles the core was running */
#define CSR_PCER_INSTR        IBEX_NumInstrRet      /* Count the number of instructions executed */
#define CSR_PCER_LD_STALL     IBEX_NumCyclesLSU     /* Number of load use hazards */
#define CSR_PCER_IMISS        IBEX_NumCyclesIF      /* Cycles waiting for instruction fetches. i.e. the number of instructions wasted due to non-ideal caches */
#define CSR_PCER_LD           IBEX_NumLoads         /* Number of memory loads executed. Misaligned accesses are counted twice */
#define CSR_PCER_ST           IBEX_NumStores        /* Number of memory stores executed. Misaligned accesses are counted twice */
#define CSR_PCER_JUMP         IBEX_NumJumps         /* Number of jump instructions seen, i.e. j, jr, jal, jalr */
#define CSR_PCER_BRANCH       IBEX_NumBranches      /* Number of branch instructions seen, i.e. bf, bnf */
#define CSR_PCER_TAKEN_BRANCH IBEX_NumBranchesTaken /* Number of taken branch instructions seen, i.e. bf, bnf */
#define CSR_PCER_RVC          IBEX_NumInstrRetC     /* Number of compressed instructions */
// Not implemented in ibex
// #define CSR_PCER_ELW          0   /* Cycles wasted due to ELW instruction */
// #define CSR_PCER_JMP_STALL    0   /* Number of jump register hazards */


// External perf counters
#define IBEX_External_BASE    13
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
    ( id == IBEX_NumCycles ? "Cycles" : \
      id == IBEX_NumInstrRet ? "Instructions" : \
      id == IBEX_NumCyclesLSU ? "LD_Stall" : \
      id == IBEX_NumCyclesIF ? "IMISS" : \
      id == IBEX_NumLoads ? "LD" : \
      id == IBEX_NumStores ? "ST" : \
      id == IBEX_NumJumps ? "JUMP" : \
      id == IBEX_NumBranches ? "BRANCH" : \
      id == IBEX_NumBranchesTaken ? "TAKEN_BRANCH" : \
      id == IBEX_NumInstrRetC ? "RVC" : \
      id == 16 ? "LD_EXT" : \
      id == 17 ? "ST_EXT" : \
      id == 18 ? "LD_EXT_CYC" : \
      id == 19 ? "ST_EXT_CYC" : \
      id == 20 ? "TCDM_CONT" : \
      "NA")

#endif
