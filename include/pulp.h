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

typedef enum {
  PI_FREQ_DOMAIN_FC     = 0,
  PI_FREQ_DOMAIN_CL     = 1,
  PI_FREQ_DOMAIN_PERIPH = 2,
  PI_FREQ_NB_DOMAINS    = 3
} pi_freq_domain_e;

#ifdef ARCHI_HMR
  void pos_hmr_store_part_to_stack(void);
  void pos_hmr_store_rest_to_stack(void);
  void pos_hmr_load_part_from_stack(void);
  void pos_hmr_store_state_to_stack(void);
  void pos_hmr_load_state_from_stack(void);
  void pos_hmr_sw_reload(void);
  void pos_hmr_tmr_irq(void);
  void pos_hmr_synch(void);
  void pos_hmr_tmr_synch_entry(void);
  void pos_hmr_tmr_synch_exit(void);
  void pos_hmr_tmr_synch(void);
  void pos_hmr_dmr_synch_entry(void);
  void pos_hmr_dmr_synch_exit(void);
  void pos_hmr_dmr_synch(void);
  int  hmr_tmr_critical_section(int (*function_handle)(void));
  int  hmr_dmr_critical_section(int (*function_handle)(void));
  void hmr_tmr_performance_section(void (*function_handle)(void));
  void hmr_tmr_perf_setup_sp(void);
  void hmr_dmr_performance_section(void (*function_handle)(void));
  void hmr_dmr_perf_setup_sp(void);
#endif

#ifdef ARCHI_HAS_CLUSTER

extern L1_DATA char *cluster_stacks;

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
