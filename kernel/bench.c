/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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

#include <stdio.h>
#include <stdlib.h>
#include "bench/bench.h"
#include "hal/pulp.h"
#include "archi/pulp.h"

// Flag for disabling the printf output
uint32_t enable_printf = 1;

void bench_disable_printf(void) {
  enable_printf = 0;
}

void bench_timer_start(void) {
  if (get_core_id()==0)
    start_timer();
#ifdef PROFILE
  perf_start();
#endif
}

void bench_timer_stop(void) {
  if (get_core_id()==0)
    stop_timer();

#ifdef PROFILE
  perf_stop();
#endif

}

void bench_timer_reset(void) {
  if (get_core_id()==0) {
    stop_timer();
    reset_timer();
  }
#ifdef PROFILE
  perf_reset();
#endif
}

void print_result(testcase_t *test, testresult_t *result)
{
  printf("== test: %s -> ", test->name);

  if (result->errors == 0)
    printf("success, ");
  else
    printf("fail, ");

  printf("nr. of errors: %d", result->errors);

  if(result->time == 0)
    printf("\n");
  else
    printf(", execution time: %d\n", result->time);

#ifdef PROFILE
  perf_print_all();
#endif
}

void print_summary(unsigned int errors)
{
  #ifdef RTL_SDK
  volatile int* ptr = (int*)(0x10001000+get_core_id()*4*2);
  ptr[1] = errors;
  #endif

  #ifdef RTL_SDK
  if (errors == 0) {
    ptr[0] = 0xABBAABBA;
  else {
    ptr[0] = 0xDEADBEEF;
  }
  #endif

  if(enable_printf) {
    printf("==== SUMMARY: ");

    if (errors == 0) {
      printf("SUCCESS\n");
    } else {
      printf("FAIL\n");
    }
  }
}

void run_benchmark(testcase_t *test, testresult_t *result)
{
  result->errors = 0;

  bench_timer_reset();

  test->test(result, bench_timer_start, bench_timer_stop);

  result->time = get_time();
}

int run_suite(testcase_t *tests)
{
  // figure out how many tests should be run
  int num = 0;
  while(tests[num].name != 0) num++;

  unsigned int errors = 0;
  int i;

  // perform the tests
  for (i = 0; i < num; i++) {
    testresult_t result;
    run_benchmark(&tests[i], &result);
    if(enable_printf && (get_core_id()==0))
      print_result(&tests[i], &result);
    errors += result.errors;
  }

  if(get_core_id() == 0)
    print_summary(errors);

  return errors;
}

void check_uint32(testresult_t* result, const char* fail_msg, uint32_t actual, uint32_t expected)
{
  if(actual != expected) {
    result->errors += 1;
    if(enable_printf) printf("%s: Actual %X, expected %X\n", fail_msg, (unsigned int) actual, (unsigned int)expected);
  }
}

void perf_print_all(void) {
#ifdef __ibex__
  printf("Perf CYCLES: %d\n",      cpu_perf_get(CSR_PCER_CYCLES));
  printf("Perf INSTR: %d\n",       cpu_perf_get(CSR_PCER_INSTR));
  printf("Perf CINSTR: %d\n",      cpu_perf_get(CSR_PCER_RVC));
  printf("Perf LD_STALL: %d\n",    cpu_perf_get(CSR_PCER_LD_STALL));
  printf("Perf JR_STALL: [Not Implemented]\n");
  printf("Perf IMISS: %d\n",       cpu_perf_get(CSR_PCER_IMISS));
  printf("Perf LD: %d\n",          cpu_perf_get(CSR_PCER_LD));
  printf("Perf ST: %d\n",          cpu_perf_get(CSR_PCER_ST));
  printf("Perf JUMP: %d\n",        cpu_perf_get(CSR_PCER_JUMP));
  printf("Perf BRANCH: %d\n",      cpu_perf_get(CSR_PCER_BRANCH));
  printf("Perf BTAKEN: %d\n",      cpu_perf_get(CSR_PCER_TAKEN_BRANCH));
  printf("Perf LD EXT: %d\n",      cpu_perf_get(CSR_PCER_LD_EXT));
  printf("Perf ST EXT: %d\n",      cpu_perf_get(CSR_PCER_ST_EXT));
  printf("Perf LD EXT CYC: %d\n",  cpu_perf_get(CSR_PCER_LD_EXT_CYC));
  printf("Perf ST EXT CYC: %d\n",  cpu_perf_get(CSR_PCER_ST_EXT_CYC));
  printf("Perf TCDM CONT: %d\n",   cpu_perf_get(CSR_PCER_TCDM_CONT));
  printf("Perf CSR HAZARD: [Not Implemented]\n");
#elif defined(__cv32e40p__)
  // not implemented registers will return 0s. Only 1,2,3 are implemented.
  printf("MCYCLE     :%d\n",    cpu_perf_get(0));
  printf("Perf reg 1 : not implemented\n")     ;
  printf("MINSTR     :%d\n",    cpu_perf_get(2));
  printf("Perf reg 3 :%d\n",    cpu_perf_get(3));
  printf("Perf reg 4 :%d\n",    cpu_perf_get(4));
  printf("Perf reg 5 :%d\n",    cpu_perf_get(5));
  printf("Perf reg 6 :%d\n",    cpu_perf_get(6));
  printf("Perf reg 7 :%d\n",    cpu_perf_get(7));
  printf("Perf reg 8 :%d\n",    cpu_perf_get(8));
  printf("Perf reg 9 :%d\n",    cpu_perf_get(9));
  printf("Perf reg 10:%d\n",    cpu_perf_get(10));
  printf("Perf reg 11:%d\n",    cpu_perf_get(11));
  printf("Perf reg 12:%d\n",    cpu_perf_get(12));
  printf("Perf reg 13:%d\n",    cpu_perf_get(13));
  printf("Perf reg 14:%d\n",    cpu_perf_get(14));
  printf("Perf reg 15:%d\n",    cpu_perf_get(15));
#elif defined( __riscv__ )
  printf("Perf CYCLES: %d\n",      cpu_perf_get(0));
  printf("Perf INSTR: %d\n",       cpu_perf_get(1));
  printf("Perf CINSTR: %d\n",      cpu_perf_get(10));
  printf("Perf LD_STALL: %d\n",    cpu_perf_get(2));
  printf("Perf JR_STALL: %d\n",    cpu_perf_get(3));
  printf("Perf IMISS: %d\n",       cpu_perf_get(4));
  printf("Perf LD: %d\n",          cpu_perf_get(5));
  printf("Perf ST: %d\n",          cpu_perf_get(6));
  printf("Perf JUMP: %d\n",        cpu_perf_get(7));
  printf("Perf BRANCH: %d\n",      cpu_perf_get(8));
  printf("Perf BTAKEN: %d\n",      cpu_perf_get(9));
  printf("Perf LD EXT: %d\n",      cpu_perf_get(11));
  printf("Perf ST EXT: %d\n",      cpu_perf_get(12));
  printf("Perf LD EXT CYC: %d\n",  cpu_perf_get(13));
  printf("Perf ST EXT CYC: %d\n",  cpu_perf_get(14));
  printf("Perf TCDM CONT: %d\n",   cpu_perf_get(15));
  printf("Perf CSR HAZARD: %d\n",  cpu_perf_get(16));
#ifdef HARD_FLOAT
  if (!rt_is_fc()) {
    printf("Perf APU_TY_CONF: %d\n", cpu_perf_get(17));
    printf("Perf APU_CONT: %d\n",    cpu_perf_get(18));
    printf("Perf APU_DEP: %d\n",     cpu_perf_get(19));
    printf("Perf APU_WB: %d\n",      cpu_perf_get(20));
  }
#endif
#else
#ifdef CSR_PCER_ALL_EVENTS_MASK
  printf("Perf CYCLES: %d\n",      cpu_perf_get(SPR_PCER_CYCLES));
  printf("Perf INSTR: %d\n",       cpu_perf_get(SPR_PCER_INSTR));
  printf("Perf LD_STALL: %d\n",    cpu_perf_get(SPR_PCER_LD_STALL));
  printf("Perf JMP_STALL: %d\n",   cpu_perf_get(SPR_PCER_JMP_STALL));
  printf("Perf IMISS: %d\n",       cpu_perf_get(SPR_PCER_IMISS));
  printf("Perf WBRANCH: %d\n",     cpu_perf_get(SPR_PCER_WBRANCH));
  printf("Perf WBRANCH_CYC: %d\n", cpu_perf_get(SPR_PCER_WBRANCH_CYC));
  printf("Perf LD: %d\n",          cpu_perf_get(SPR_PCER_LD));
  printf("Perf ST: %d\n",          cpu_perf_get(SPR_PCER_ST));
  printf("Perf JUMP: %d\n",        cpu_perf_get(SPR_PCER_JUMP));
  printf("Perf BRANCH: %d\n",      cpu_perf_get(SPR_PCER_BRANCH));
  printf("Perf DELAY NOP: %d\n",   cpu_perf_get(SPR_PCER_DELAY_NOP));
  printf("Perf LD EXT: %d\n",      cpu_perf_get(SPR_PCER_LD_EXT));
  printf("Perf ST EXT: %d\n",      cpu_perf_get(SPR_PCER_ST_EXT));
  printf("Perf LD EXT CYC: %d\n",  cpu_perf_get(SPR_PCER_LD_EXT_CYC));
  printf("Perf ST EXT CYC: %d\n",  cpu_perf_get(SPR_PCER_ST_EXT_CYC));
  printf("Perf TCDM CONT: %d\n",   cpu_perf_get(SPR_PCER_TCDM_CONT));
#endif
#endif

  // new ICACHE handles this differently
  // // icache
  // int i;
  // for(i = 0; i < 4; i++) {
  //   printf("I$ bank %d: Read hits %d\n", i, read_hit_icache_reg(i));
  //   printf("I$ bank %d: Trans %d\n",     i, read_trans_icache_reg(i));
  // }
}


// use weak attribute here, so we can overwrite this function to provide custom
// exception handlers, e.g. for tests
//__attribute__((interrupt))
__attribute__((weak))
void illegal_insn_handler_c(void)
{
#ifndef __ariane__
  unsigned int exception_address, insn;
#if defined( __riscv__ ) || defined( __ibex__) || defined(__cv32e40p__)
  asm("csrr %0, 0x341" : "=r" (exception_address) : );
#else
  exception_address = hal_spr_read(SPR_EPCR_BASE);
#endif
  insn = *((unsigned int*)(exception_address));
  printf("Illegal instruction encountered at address 0x%08X: %X\n", exception_address, insn);
  exit(1);
#endif
}


int bench_cluster_exec(int cid, int (*entry)())
{
  if (rt_cluster_id() != (unsigned int)cid)
  {
    cluster_start(cid, entry);

    return cluster_wait(cid);
  }

  return 0;
}

extern int main(int argc, const char * const argv[]);

int bench_cluster_forward(int cid)
{
  #ifdef ARCHI_NO_FC
  return 0;
  #else
  return bench_cluster_exec(cid, main);
  #endif
}
