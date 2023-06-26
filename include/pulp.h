/*
 * Copyright (C) 2019 ETH Zurich and University of Bologna
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

#ifndef __PULP_H__
#define __PULP_H__

#ifndef CLUSTER_STACK_SIZE
#define CLUSTER_STACK_SIZE 0x800
#endif

#ifndef LANGUAGE_ASSEMBLY

#include <bench/bench.h>
#include <archi/pulp.h>
#include <hal/pulp.h>
#include <data/data.h>


#include "keccak/keccak_auto.h"
#include "keccak/keccak_driver.h"

#include "ntt_intt_pwm/ntt_intt_pwm_auto.h"
#include "ntt_intt_pwm/ntt_intt_pwm_driver.h"
#include "ntt_intt_pwm/intt_driver.h"
#include "ntt_intt_pwm/pwm_driver.h"

//#include "SHA3/fips_202.h"


//KYBER768
#include "kyber768/api.h"
#include "kyber768/cbd.h"
#include "kyber768/fips202.h"
#include "kyber768/indcpa.h"
#include "kyber768/kem.h"
#include "kyber768/ntt.h"
#include "kyber768/params.h"
#include "kyber768/poly.h"
#include "kyber768/polyvec.h"
#include "kyber768/randombytes.h"
#include "kyber768/reduce.h"
#include "kyber768/symmetric.h"
#include "kyber768/verify.h"

//KYBER512
/*#include "kyber512/api.h"
#include "kyber512/cbd.h"
#include "kyber512/fips202.h"
#include "kyber512/indcpa.h"
#include "kyber512/kem.h"
#include "kyber512/ntt.h"
#include "kyber512/params.h"
#include "kyber512/poly.h"
#include "kyber512/polyvec.h"
#include "kyber512/randombytes.h"
#include "kyber512/reduce.h"
#include "kyber512/symmetric.h"
#include "kyber512/verify.h"*/

//KYBER1024
/*
#include "kyber1024/api.h"
#include "kyber1024/cbd.h"
#include "kyber1024/fips202.h"
#include "kyber1024/indcpa.h"
#include "kyber1024/kem.h"
#include "kyber1024/ntt.h"
#include "kyber1024/params.h"
#include "kyber1024/poly.h"
#include "kyber1024/polyvec.h"
#include "kyber1024/randombytes.h"
#include "kyber1024/reduce.h"
#include "kyber1024/symmetric.h"
#include "kyber1024/verify.h"
*/

typedef enum {
  PI_FREQ_DOMAIN_FC     = 0,
  PI_FREQ_DOMAIN_CL     = 1,
  PI_FREQ_DOMAIN_PERIPH = 2,
  PI_FREQ_NB_DOMAINS    = 3
} pi_freq_domain_e;

#ifdef ARCHI_HAS_CLUSTER

void cluster_start(int cid, int (*entry)());

void cluster_entry_stub();

int cluster_wait(int cid);

#else

static inline void cluster_start(int cid, int (*entry)())
{
}

static inline int cluster_wait(int cid)
{
  return 0;
}

#endif

void _start();

#include <implem/implem.h>

#define get_core_id hal_core_id
#define rt_core_id hal_core_id
#define rt_cluster_id hal_cluster_id
#define get_cluster_id hal_cluster_id

#ifdef ARCHI_CLUSTER_NB_PE
static inline int get_core_num() { return ARCHI_CLUSTER_NB_PE; }
#endif


int uart_open(int uart_id, int baudrate);
void uart_close(int uart_id);
int uart_write(int uart_id, void *buffer, uint32_t size);
int uart_read(int uart_id, void *buffer, uint32_t size);

void synch_barrier();


void *pi_l1_malloc(int cid, int size);
void pi_l1_free(int cid, void *chunk, int size);

void *pi_l2_malloc(int size);
void pi_l2_free(void *_chunk, int size);




#endif

#endif
