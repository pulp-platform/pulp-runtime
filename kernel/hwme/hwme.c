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

/* 
 * Authors:  Francesco Conti <fconti@iis.ee.ethz.ch>
 */

#include "pulp.h"
#include <stdint.h>
#include "archi/hwme/hwme_v1.h"
#include "hal/hwme/hwme_v1.h"

#define USE_STIMULI
// comment below line to run only dot product with bias
//#define DO_MATVEC_MULT
#ifndef DO_MATVEC_MULT
    #define DO_DOT_PROD
#endif

#include "hwme_stimuli_a.h"
#include "hwme_stimuli_b.h"
#include "hwme_stimuli_c.h"
#include "hwme_stimuli_d.h"

int main() {

  uint32_t *a = (uint8_t *) 0x1c010000;
  uint32_t *b = (uint8_t *) 0x1c010200;
  uint32_t *c = (uint8_t *) 0x1c010400;
  uint32_t *d = (uint8_t *) 0x1c010600;

  int coreID = get_core_id();
#ifdef DO_MATVEC_MULT
  // define dimensions
  uint32_t in_vec_len = 8;
  uint32_t out_vec_len = 10;
#endif

  volatile int errors = 0;
  int gold_sum = 0, check_sum = 0;
  int i,j;
  
  int offload_id_tmp, offload_id;

  if(get_core_id() == 0) {

#ifdef USE_STIMULI
    for(int i=0; i<512; i++) {
      ((uint8_t *) a)[i] = stim_a[i];
    }
    for(int i=0; i<512; i++) {
      ((uint8_t *) b)[i] = stim_b[i];
    }
    for(int i=0; i<512; i++) {
#ifdef DO_MATVEC_MULT
      ((uint8_t *) c)[i] = 0; // no bias for matrix vector multiplication
#else
      ((uint8_t *) c)[i] = stim_c[i];
#endif
    }
    for(int i=0; i<512; i++) {
      ((uint8_t *) d)[i] = stim_d[i];
    }
#else
    for(int i=0; i<128; i++) {
      a[i] = i;
    }
    for(int i=0; i<128; i++) {
      b[i] = i;
    }
    for(int i=0; i<128; i++) {
      c[i] = i;
    }
    for(int i=0; i<128; i++) {
      d[i] = i;
    }
#endif

    /* convolution-accumulation - HW */
    plp_hwme_enable();

    while((offload_id_tmp = hwme_acquire_job()) < 0);

    // set up bytecode
    hwme_bytecode_set(HWME_LOOPS1_OFFS,           0x00000000);
    hwme_bytecode_set(HWME_BYTECODE5_LOOPS0_OFFS, 0x00040000);
    hwme_bytecode_set(HWME_BYTECODE4_OFFS,        0x00000000);
    hwme_bytecode_set(HWME_BYTECODE3_OFFS,        0x00000000);
    hwme_bytecode_set(HWME_BYTECODE2_OFFS,        0x00000000);
    hwme_bytecode_set(HWME_BYTECODE1_OFFS,        0x000008cd);
    hwme_bytecode_set(HWME_BYTECODE0_OFFS,        0x11a13c05);
    
    // job-dependent registers
    hwme_a_addr_set((unsigned int) a);
    hwme_b_addr_set((unsigned int) b);
    hwme_c_addr_set((unsigned int) c);
    hwme_d_addr_set((unsigned int) d);
#ifdef DO_MATVEC_MULT
    hwme_nb_iter_set(out_vec_len);
    hwme_len_iter_set(in_vec_len-1);
    hwme_vectstride_set(in_vec_len*4); // stride for the matrix is equal to in_vec length * wordsize
    hwme_vectstride2_set(0); // stride for the vector is zero
#else
    hwme_nb_iter_set(4);
    hwme_len_iter_set(32-1);
    hwme_vectstride_set(32*4);
    hwme_vectstride2_set(32*4); // same stride for both streams
#endif
    hwme_shift_simplemul_set(hwme_shift_simplemul_value(0, 0));

    // start HWME operation
    hwme_trigger_job();

    // wait for end of compuation
    soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_FCHWPE0);
    __rt_periph_wait_event(ARCHI_SOC_EVENT_FCHWPE0, 1);

    plp_hwme_disable();

    // check
#ifndef USE_STIMULI
    if(d[0] != 0x000028b0) errors++;
    if(d[1] != 0x000124b1) errors++;
    if(d[2] != 0x000320b2) errors++;
    if(d[3] != 0x00061cb3) errors++;
#else
    #ifdef DO_MATVEC_MULT
        if(d[0] != 0x7CB12A38) errors++;
        if(d[1] != 0xCD4F4DCB) errors++;
        if(d[2] != 0x49CD5D5C) errors++;
        if(d[3] != 0x2A1D8706) errors++;
    #else
        if(d[0] != 0x7f228fd6) errors++;
        if(d[1] != 0x23a7d5c2) errors++;
        if(d[2] != 0x7f281848) errors++;
        if(d[3] != 0x6127d834) errors++;
    #endif
#endif /* USE_STIMULI */

    printf("errors=%d\n", errors);

   }
   synch_barrier();

   return errors;
}
