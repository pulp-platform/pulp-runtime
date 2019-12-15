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

volatile void *cluster_entry;

L1_DATA char cluster_stacks[ARCHI_CLUSTER_NB_PE*CLUSTER_STACK_SIZE];



static void pos_wait_forever()
{
    eu_evt_maskClr(0xffffffff);
    eu_evt_wait();
    while(1);
}



void cluster_entry_stub()
{
    int retval = ((int (*)())cluster_entry)();

    if (hal_core_id() == 0)
    {
        exit(retval);
    }
    else
        pos_wait_forever();
}


void cluster_start(int cid, int (*entry)(void *))
{
    // Store cluster entry point, ctr0 will jump here
    cluster_entry = entry;

    // Init FLL
    pos_fll_init(POS_FLL_CL);
      
    // Initialize cluster L1 memory allocator
    alloc_init_l1(cid);

    // Activate icache
    hal_icache_cluster_enable(cid);


    // Fetch all cores
    for (int i=0; i<ARCHI_CLUSTER_NB_PE; i++)
    {
      plp_ctrl_core_bootaddr_set_remote(cid, i, (int)_start);
    }

    eoc_fetch_enable_remote(cid, (1<<ARCHI_CLUSTER_NB_PE) - 1);
}


int cluster_wait(int cid)
{
    printf("%s %d\n", __FILE__, __LINE__);
    while(1);
    return 0;
}
