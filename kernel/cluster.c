/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna
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

#include "pulp.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef ARCHI_NO_FC
#include "init.c"
#endif

volatile void *cluster_entry;

L1_DATA char *cluster_stacks;

static volatile int cluster_running;
static volatile int cluster_retval;

extern int main(int argc, const char * const argv[]);

static void pos_wait_forever()
{
    eu_evt_maskClr(0xffffffff);
    eu_evt_wait();
    while(1);
}



static void cluster_core_init()
{
    eu_evt_maskSet((1<<PULP_DISPATCH_EVENT) | (1<<PULP_MUTEX_EVENT) | (1<<PULP_HW_BAR_EVENT));

    eu_bar_setup(eu_bar_addr(0), (1<<ARCHI_CLUSTER_NB_PE) - 1);
}

// executed by the FC!
void cluster_entry_stub()
{
    cluster_core_init();

#ifdef ARCHI_NO_FC
    synch_barrier();

    if(hal_core_id()==0)
      cluster_start(hal_cluster_id(), main);

    synch_barrier();

#endif

    int retval = ((int (*)())cluster_entry)();

    if (hal_core_id() == 0)
    {
        cluster_retval = retval;
        cluster_running = 0;
    }

    pos_wait_forever();
}


void cluster_start(int cid, int (*entry)())
{
    // Store cluster entry point, ctr0 will jump here
    cluster_entry = entry;

    // Init FLL only if the FC is present
#ifndef ARCHI_NO_FC
    pos_fll_init(POS_FLL_CL);
#endif

    // Initialize cluster L1 memory allocator
    alloc_init_l1(cid);

    // Activate icache
    hal_icache_cluster_enable(cid);

#ifdef ARCHI_NO_FC
    if (!hal_is_fc())
        cluster_core_init();
#endif

    alloc_init_l1(cid);

    cluster_stacks = pi_l1_malloc(cid, ARCHI_CLUSTER_NB_PE*CLUSTER_STACK_SIZE);

#ifdef ARCHI_NO_FC
    if (cluster_stacks == NULL)
        return;

    cluster_running = 1;

  // Fetch all cores
    for (int i=0; i<ARCHI_CLUSTER_NB_PE; i++)
    {
      plp_ctrl_core_bootaddr_set_remote(cid, i, (int)_start);
    }

    eoc_fetch_enable_remote(cid, (1<<ARCHI_CLUSTER_NB_PE) - 1);
#endif
}


int cluster_wait(int cid)
{
    while(cluster_running);

    return cluster_retval;
}

void synch_barrier()
{
#ifdef ARCHI_FC_CID
    if (hal_cluster_id() != ARCHI_FC_CID)
#endif
    {
        eu_bar_trig_wait_clr(eu_bar_addr(0));
    }
}
